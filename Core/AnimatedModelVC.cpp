#include "pch.h"
#include "AnimatedModelVC.h"
#include "Application.h"
#include "Material3D.h"
#include "Object.h"
#include "Scene.h"
#include "CameraRMC.h"
#include "Model.h"
#include "Canvas.h"
#include "d3dx12.h"

using namespace Ion::Core;

AnimatedModelVC::AnimatedModelVC(const std::string& modelName, const std::string& materialName, bool isActive, Winding winding, CoordSystem coordSystem, Object* pObject)
	: ModelVC(modelName, materialName, isActive, winding, coordSystem, pObject)
	, mTransforms{}
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
	mTransforms.reserve(128);
	DirectX::XMFLOAT4X4 identity{ 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f };
	mTransforms.assign(128, identity);
	mpBonesConstantBufferData = (BonesConstantBuffer *)&mTransforms[0];
}

AnimatedModelVC::~AnimatedModelVC()
{
}

void AnimatedModelVC::SetAnimation(const AnimationClip& animationClip)
{
	mAnimationClip = animationClip;
	mIsClipSet = true;
}

void AnimatedModelVC::SetAnimation(size_t clipNumber)
{
	if (clipNumber >= mpModel->GetAnimationClips().size())
		return;
	SetAnimation(mpModel->GetAnimationClips()[clipNumber]);
}

void AnimatedModelVC::SetIsAnimating(bool isAnimating)
{
	mIsAnimating = isAnimating;
	if (!mIsAnimating || !mIsClipSet)
		return;
	mTransforms.assign(mAnimationClip.GetKeys()[0].GetBoneTransforms().begin(), mAnimationClip.GetKeys()[0].GetBoneTransforms().end());
}

void AnimatedModelVC::Initialize()
{
	ModelVC::Initialize();
	Application* pApplication{ mpObject->GetScene()->GetApplication() };
	auto pDevice{ pApplication->GetDevice() };

	{
		D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc{};
		cbvHeapDesc.NumDescriptors = 1;
		cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		ThrowIfFailed(pDevice->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&mpBonesCbvHeap)));
	}
	{
		const UINT bonesConstantBufferSize{ sizeof(BonesConstantBuffer) };

		D3D12_HEAP_PROPERTIES heapProp{ CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD) };
		D3D12_RESOURCE_DESC resDesc{ CD3DX12_RESOURCE_DESC::Buffer(bonesConstantBufferSize) };
		ThrowIfFailed(pDevice->CreateCommittedResource(
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
		ThrowIfFailed(mpBonesConstantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&mpBonesCbvDataBegin)));
		memcpy(mpBonesCbvDataBegin, mpBonesConstantBufferData, sizeof(BonesConstantBuffer));
	}
	mIsInitialized = true;
}

void AnimatedModelVC::Update(float delta)
{
	if (!mIsActive)
		return;
	ModelVC::Update(delta);
	if (!(mIsAnimating && mIsClipSet))
		return;

	float passedTicks{ delta * mAnimationClip.GetTicksPerSecond() * mAnimationSpeed };
	if (passedTicks > mAnimationClip.GetDuration())
		passedTicks = mAnimationClip.GetDuration();

	mTickCount += passedTicks;
	if (mTickCount > mAnimationClip.GetDuration())
		mTickCount -= mAnimationClip.GetDuration();

	AnimationKey
		keyA{ mAnimationClip.GetKeys().front() },
		keyB{ mAnimationClip.GetKeys().front() };

	for (AnimationKey& key : mAnimationClip.GetKeys())
	{
		if (key.GetTick() > mTickCount)
		{
			keyB = key;
			break;
		}
		keyA = key;
	}

	float blendB{ (mTickCount - keyA.GetTick()) / (keyB.GetTick() - keyA.GetTick()) };

	mTransforms.clear();

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
		mTransforms.push_back(resultFloat4x4);
	}
}

void AnimatedModelVC::Render(Canvas* pCanvas, Material3D* pMaterial)
{
	(pMaterial);
	if (!mIsActive)
		return;

	Application* pApplication{ mpObject->GetScene()->GetApplication() };
	auto pDxgiFactory{ pApplication->GetDxgiFactory() };
	auto pDevice{ pApplication->GetDevice() };
	auto pCmdQueue{ pApplication->GetCommandQueue() };
	auto pCmdAlloc{ pApplication->GetCommandAllocator() };
	auto pGraphicsCommandList{ pCanvas->GetGraphicsCommandList() };

	DirectX::XMMATRIX world{ DirectX::XMLoadFloat4x4(&mpObject->GetModelC<TransformMC>()->GetWorld()) };
	const DirectX::XMMATRIX viewProjection{ DirectX::XMLoadFloat4x4(&pCanvas->GetCamera()->GetModelC<CameraRMC>()->GetViewProjection()) };
	DirectX::XMMATRIX wvp{ world * viewProjection };

	DirectX::XMStoreFloat4x4(&mObjectConstantBufferData.mWorld, world);
	DirectX::XMStoreFloat4x4(&mObjectConstantBufferData.mWorldViewProj, wvp);

	for (auto& pair : mpTextureSrvHeaps)
	{
		ID3D12DescriptorHeap* ppHeaps[]{ pair.second.Get() };
		pGraphicsCommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		CD3DX12_GPU_DESCRIPTOR_HANDLE tex{ pair.second->GetGPUDescriptorHandleForHeapStart() };
		pGraphicsCommandList->SetGraphicsRootDescriptorTable(UINT(pair.first) + 3, tex);
	}
	memcpy(mpObjectCbvDataBegin, &mObjectConstantBufferData, sizeof(mObjectConstantBufferData));
	{
		ID3D12DescriptorHeap* ppHeaps[]{ mpObjectCbvHeap.Get() };
		pGraphicsCommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		pGraphicsCommandList->SetGraphicsRootDescriptorTable(1, mpObjectCbvHeap->GetGPUDescriptorHandleForHeapStart());
	}
	memcpy(mpBonesCbvDataBegin, mpBonesConstantBufferData, sizeof(BonesConstantBuffer));
	{
		ID3D12DescriptorHeap* ppHeaps[]{ mpBonesCbvHeap.Get() };
		pGraphicsCommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		pGraphicsCommandList->SetGraphicsRootDescriptorTable(2, mpBonesCbvHeap->GetGPUDescriptorHandleForHeapStart());
	}
	pGraphicsCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pGraphicsCommandList->IASetIndexBuffer(&mIndexBufferView);
	pGraphicsCommandList->IASetVertexBuffers(0, 1, &mVertexBufferView);
	pGraphicsCommandList->DrawIndexedInstanced(UINT(mIndexCount), 1, 0, 0, 0);
}
