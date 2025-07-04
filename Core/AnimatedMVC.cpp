#include "pch.h"
#include "AnimatedMVC.h"
#include "Application.h"
#include "Material3D.h"
#include "Object.h"
#include "Scene.h"
#include "CameraRMC.h"
#include "MeshModel.h"
#include "Canvas.h"

using namespace Ion;

Core::AnimatedMVC::AnimatedMVC(const std::string& modelName, const std::string& modelExtension, const std::string& materialName, bool isActive, Core::Winding winding, Core::CoordSystem coordSystem, Core::Object* pObject)
	: Core::MeshModelVC(modelName, modelExtension, materialName, isActive, winding, coordSystem, pObject)
	, mBoneTransforms{}
	, mAnimationClip{}
	, mTickCount{ 0.f }
	, mAnimationSpeed{ 1.f }
	, mIsAnimating{ false }
	, mIsClipSet{ false }
        , mpBonesConstantBuffer{}
        , mpBonesConstantBufferData{}
        , mpBonesCbvDataBegin{ nullptr }
{
	mBoneTransforms.reserve(128);
	DirectX::XMFLOAT4X4 identity{ 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f };
	mBoneTransforms.assign(128, identity);
	mpBonesConstantBufferData = (BonesConstantBuffer *)&mBoneTransforms[0];
}

void Core::AnimatedMVC::SetAnimation(const Core::AnimationClip& animationClip)
{
	mAnimationClip = animationClip;
	mIsClipSet = true;
}

void Core::AnimatedMVC::SetAnimation(std::size_t clipNumber)
{
	if (clipNumber >= mpMeshModel->GetAnimationClips().size())
		return;
	SetAnimation(mpMeshModel->GetAnimationClips()[clipNumber]);
}

void Core::AnimatedMVC::SetIsAnimating(bool isAnimating)
{
	mIsAnimating = isAnimating;
	if (!mIsAnimating || !mIsClipSet)
		return;
	mBoneTransforms.assign(mAnimationClip.GetKeys()[0].GetBoneTransforms().begin(), mAnimationClip.GetKeys()[0].GetBoneTransforms().end());
}

void Core::AnimatedMVC::Initialize()
{
	Core::MeshModelVC::Initialize();
	Core::Application* pApplication{ mpObject->GetScene()->GetApplication() };
	auto pDevice{ pApplication->GetDevice() };

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

                CD3DX12_RANGE readRange(0, 0);
                mpObject->GetScene()->GetApplication()->ThrowIfFailed(mpBonesConstantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&mpBonesCbvDataBegin)));
                memcpy(mpBonesCbvDataBegin, mpBonesConstantBufferData, sizeof(Core::BonesConstantBuffer));
        }
	mIsInitialized = true;
}

void Core::AnimatedMVC::Update(float delta)
{
	Core::MeshModelVC::Update(delta);
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

	std::size_t boneCount{ keyA.GetBoneTransforms().size() };
	for (std::size_t bone{ 0 }; bone < boneCount; ++bone)
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

bool Core::AnimatedMVC::Render(Core::Canvas* pCanvas, Core::Material3D* pMaterial, float distSq)
{
	if (!mIsInitialized)
	{
#ifdef ION_LOGGER
		mpObject->GetScene()->GetApplication()->GetServiceLocator().GetLogger()->Message(typeid(this).name(), Core::MsgType::Fatal, "AnimatedModelVC.Render() while mIsInitialized == false");
#endif
		return false;
	}
	if (!Core::ViewC::Render(pCanvas, pMaterial, distSq))
		return false;
	if (!mIsActive)
		return false;

        auto pGraphicsCommandList{ pCanvas->GetGraphicsCommandList() };

        auto pApplication{ mpObject->GetScene()->GetApplication() };
        UINT dsTable{ 1 };
        // Bind canvas constant buffer descriptor table
        pGraphicsCommandList->SetGraphicsRootDescriptorTable(0, pApplication->GetGpuHandle(mCbvSrvOffset));
        SetDescTableObjectConstants(pCanvas, dsTable);

        // Upload bone matrices and bind as root CBV
        memcpy(mpBonesCbvDataBegin, mpBonesConstantBufferData, sizeof(Core::BonesConstantBuffer));
        pGraphicsCommandList->SetGraphicsRootConstantBufferView(dsTable, mpBonesConstantBuffer->GetGPUVirtualAddress());
        ++dsTable;

        SetDescTableTextures(pCanvas, dsTable);

	pGraphicsCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pGraphicsCommandList->IASetIndexBuffer(&mIndexBufferView);
	pGraphicsCommandList->IASetVertexBuffers(0, 1, &mVertexBufferView);
	pGraphicsCommandList->DrawIndexedInstanced(UINT(mIndexCount), 1, 0, 0, 0);
	return true;
}
