#include "pch.h"
#include "InstancedAnimatedModelVC.h"
#include "Application.h"
#include "Material3D.h"
#include "Object.h"
#include "Scene.h"
#include "CameraRMC.h"
#include "Model.h"
#include "Canvas.h"
#include "d3dx12.h"

using namespace Ion;

const size_t Core::InstancedAnimatedModelVC::mMaxInstances{ 40960 };

Core::InstancedAnimatedModelVC::InstancedAnimatedModelVC(const std::string& modelName, const std::string& modelExtension, const std::string& materialName, bool isActive, Core::Winding winding, Core::CoordSystem coordSystem, Core::Object* pObject)
	: ModelVC(modelName, modelExtension, materialName, isActive, winding, coordSystem, pObject)
	, mTransforms{}
	, mpInstanceBuffer{}
	, mInstanceBufferData{}
	, mpInstanceDataBegin{ nullptr }
	, mBoneTransforms{}
	, mAnimationClip{}
	, mTickCount{ 0.f }
	, mAnimationSpeed{ 1.f }
	, mIsAnimating{ false }
	, mIsClipSet{ false }
	, mpBonesCbvHeap{}
	, mpBonesConstantBuffer{}
	, mpBonesConstantBufferData{}
	, mpBonesCbvDataBegin{ nullptr }
{
	mBoneTransforms.reserve(128);
	DirectX::XMFLOAT4X4 identity{ 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f };
	mBoneTransforms.assign(128, identity);
	mpBonesConstantBufferData = (BonesConstantBuffer*)&mBoneTransforms[0];
}

Core::InstancedAnimatedModelVC::~InstancedAnimatedModelVC()
{
}

void Core::InstancedAnimatedModelVC::AddInstance(const Core::TransformMC& transformMC)
{
	mTransforms.push_back(transformMC);
	mTransforms.back().Update(0.f);
	mInstanceBufferData.emplace_back(mTransforms.back().GetWorld());
}

void Core::InstancedAnimatedModelVC::AddInstances(const std::vector<Core::TransformMC>& transformMCs)
{
	for (auto& transformMC : transformMCs)
		AddInstance(transformMC);
}

void Core::InstancedAnimatedModelVC::ReadInstances()
{
	if (!mTransforms.empty())
		return;
	const std::vector<Core::Transform>& transforms{ mpModel->ReadInstances() };
	Core::TransformMC transformMC{ true, mpObject };
	for (auto& transform : transforms)
	{
		transformMC.SetPosition(transform.mPosition);
		transformMC.SetScale(transform.mScale);
		transformMC.SetRotation(transform.mRotation);
		AddInstance(transformMC);
	}
}

std::vector<Core::TransformMC>& Core::InstancedAnimatedModelVC::GetInstances()
{
	return mTransforms;
}

void Core::InstancedAnimatedModelVC::SetAnimation(const Core::AnimationClip& animationClip)
{
	mAnimationClip = animationClip;
	mIsClipSet = true;
}

void Core::InstancedAnimatedModelVC::SetAnimation(size_t clipNumber)
{
	if (clipNumber >= mpModel->GetAnimationClips().size())
		return;
	SetAnimation(mpModel->GetAnimationClips()[clipNumber]);
}

void Core::InstancedAnimatedModelVC::SetIsAnimating(bool isAnimating)
{
	mIsAnimating = isAnimating;
	if (!mIsAnimating || !mIsClipSet)
		return;
	mBoneTransforms.assign(mAnimationClip.GetKeys()[0].GetBoneTransforms().begin(), mAnimationClip.GetKeys()[0].GetBoneTransforms().end());
}

void Core::InstancedAnimatedModelVC::Initialize()
{
	Core::ModelVC::Initialize();
	Core::Application* pApplication{ mpObject->GetScene()->GetApplication() };
	auto pDevice{ pApplication->GetDevice() };
	{
		D3D12_HEAP_PROPERTIES heapProp{ CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD) };
		D3D12_RESOURCE_DESC resDesc{ CD3DX12_RESOURCE_DESC::Buffer(sizeof(Core::InstanceBuffer) * mMaxInstances) };
		mpObject->GetScene()->GetApplication()->ThrowIfFailed(pDevice->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mpInstanceBuffer)));

		CD3DX12_RANGE readRange(0, 0);
		mpObject->GetScene()->GetApplication()->ThrowIfFailed(mpInstanceBuffer->Map(0, &readRange, reinterpret_cast<void**>(&mpInstanceDataBegin)));
		memcpy(mpInstanceDataBegin, mInstanceBufferData.data(), sizeof(Core::InstanceBuffer) * mInstanceBufferData.size());
	}
	{
		D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc{};
		cbvHeapDesc.NumDescriptors = 1;
		cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		mpObject->GetScene()->GetApplication()->ThrowIfFailed(pDevice->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&mpBonesCbvHeap)));
	}
	{
		const UINT bonesConstantBufferSize{ sizeof(Core::BonesConstantBuffer) };

		D3D12_HEAP_PROPERTIES heapProp{ CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD) };
		D3D12_RESOURCE_DESC resDesc{ CD3DX12_RESOURCE_DESC::Buffer(bonesConstantBufferSize) };
		mpObject->GetScene()->GetApplication()->ThrowIfFailed(pDevice->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mpBonesConstantBuffer)));

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
		cbvDesc.BufferLocation = mpBonesConstantBuffer->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = bonesConstantBufferSize;
		pDevice->CreateConstantBufferView(&cbvDesc, mpBonesCbvHeap->GetCPUDescriptorHandleForHeapStart());

		CD3DX12_RANGE readRange(0, 0);
		mpObject->GetScene()->GetApplication()->ThrowIfFailed(mpBonesConstantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&mpBonesCbvDataBegin)));
		memcpy(mpBonesCbvDataBegin, mpBonesConstantBufferData, sizeof(Core::BonesConstantBuffer));
	}
	mIsInitialized = true;
}

void Core::InstancedAnimatedModelVC::Update(float delta)
{
	if (!mIsActive)
		return;
	Core::ModelVC::Update(delta);
	if (!(mIsAnimating && mIsClipSet))
		return;

	float passedTicks{ delta * mAnimationClip.GetTicksPerSecond() * mAnimationSpeed };
	if (passedTicks > mAnimationClip.GetDuration())
		passedTicks = mAnimationClip.GetDuration();

	mTickCount += passedTicks;
	if (mTickCount > mAnimationClip.GetDuration())
		mTickCount -= mAnimationClip.GetDuration();

	Core::AnimationKey
		keyA{ mAnimationClip.GetKeys().front() },
		keyB{ mAnimationClip.GetKeys().front() };

	for (Core::AnimationKey& key : mAnimationClip.GetKeys())
	{
		if (key.GetTick() > mTickCount)
		{
			keyB = key;
			break;
		}
		keyA = key;
	}

	float blendB{ (mTickCount - keyA.GetTick()) / (keyB.GetTick() - keyA.GetTick()) };

	mBoneTransforms.clear();

	size_t boneCount{ keyA.GetBoneTransforms().size() };
	for (size_t bone{ 0 }; bone < boneCount; ++bone)
	{
		DirectX::XMFLOAT4X4 trsA{ keyA.GetBoneTransforms()[bone] };
		DirectX::XMFLOAT4X4 trsB{ keyB.GetBoneTransforms()[bone] };
		DirectX::XMMATRIX matrixA{ DirectX::XMLoadFloat4x4(&trsA) };
		DirectX::XMVECTOR vecScaleA, vecRotA, vecTransA;
		DirectX::XMMatrixDecompose(&vecScaleA, &vecRotA, &vecTransA, matrixA);
		DirectX::XMMATRIX matrixB{ DirectX::XMLoadFloat4x4(&trsB) };
		DirectX::XMVECTOR vecScaleB, vecRotB, vecTransB;
		DirectX::XMMatrixDecompose(&vecScaleB, &vecRotB, &vecTransB, matrixB);
		DirectX::XMVECTOR vecScaleAB{ DirectX::XMVectorLerp(vecScaleA, vecScaleB, blendB) };
		DirectX::XMVECTOR vecTransAB{ DirectX::XMVectorLerp(vecTransA, vecTransB, blendB) };
		DirectX::XMVECTOR vecRotAB{ DirectX::XMQuaternionSlerp(vecRotA, vecRotB, blendB) };
		DirectX::XMMATRIX resultMatrix{ DirectX::XMMatrixTransformation(
			DirectX::FXMVECTOR{},
			DirectX::FXMVECTOR{},
			vecScaleAB,
			DirectX::FXMVECTOR{},
			vecRotAB,
			vecTransAB) };
		DirectX::XMFLOAT4X4 resultFloat4x4{};
		DirectX::XMStoreFloat4x4(&resultFloat4x4, resultMatrix);
		mBoneTransforms.push_back(resultFloat4x4);
	}
}

void Core::InstancedAnimatedModelVC::Render(Core::Canvas* pCanvas, Core::Material3D* pMaterial)
{
#ifdef _DEBUG
	if (!mIsInitialized)
	{
		mpObject->GetScene()->GetApplication()->GetServiceLocator().GetLogger()->Message(this, Core::MsgType::Fatal, "InstancedModelVC.Render() while mIsInitialized == false");
		return;
	}
#endif
	(pMaterial);
	if (!mIsActive)
		return;

	auto pGraphicsCommandList{ pCanvas->GetGraphicsCommandList() };

	UINT dsTable{ 1 };
	SetDescTableObjectConstants(pCanvas, dsTable);
	memcpy(mpBonesCbvDataBegin, mpBonesConstantBufferData, sizeof(Core::BonesConstantBuffer));
	{
		ID3D12DescriptorHeap* ppHeaps[]{ mpBonesCbvHeap.Get() };
		pGraphicsCommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		pGraphicsCommandList->SetGraphicsRootDescriptorTable(dsTable, mpBonesCbvHeap->GetGPUDescriptorHandleForHeapStart());
		++dsTable;
	}
	SetDescTableTextures(pCanvas, dsTable);
	memcpy(mpInstanceDataBegin, mInstanceBufferData.data(), sizeof(Core::InstanceBuffer) * mInstanceBufferData.size());
	pGraphicsCommandList->SetGraphicsRootShaderResourceView(dsTable, mpInstanceBuffer->GetGPUVirtualAddress());

	pGraphicsCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pGraphicsCommandList->IASetIndexBuffer(&mIndexBufferView);
	pGraphicsCommandList->IASetVertexBuffers(0, 1, &mVertexBufferView);
	pGraphicsCommandList->DrawIndexedInstanced(UINT(mIndexCount), UINT(mInstanceBufferData.size()), 0, 0, 0);
}