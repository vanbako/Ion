#include "pch.h"
#include "InstancedMVC.h"
#include "Application.h"
#include "Material3D.h"
#include "Object.h"
#include "Scene.h"
#include "CameraRMC.h"
#include "Model.h"
#include "Canvas.h"
#include "InstancedTransformMC.h"

using namespace Ion;

Core::InstancedMVC::InstancedMVC(const std::string& modelName, const std::string& modelExtension, const std::string& materialName, bool isActive, Core::Winding winding, Core::CoordSystem coordSystem, Core::Object* pObject)
	: MeshModelVC(modelName, modelExtension, materialName, isActive, winding, coordSystem, pObject)
	, mpInstancedTransform{ nullptr }
	, mpInstanceBuffer{}
	, mInstanceBufferData{}
	, mpInstanceDataBegin{ nullptr }
{
}

//void Core::InstancedMVC::AddInstance(const Core::TransformMC& transformMC)
//{
//	mTransforms.push_back(transformMC).Update(0.f);
//	mInstanceBufferData.emplace_back(mTransforms.back().GetWorld());
//}
//
//void Core::InstancedMVC::AddInstances(const std::vector<Core::TransformMC>& transformMCs)
//{
//	for (auto& transformMC : transformMCs)
//		AddInstance(transformMC);
//}

void Core::InstancedMVC::SetInstancedTransform(InstancedTransformMC* pInstancedTransform)
{
	mpInstancedTransform = pInstancedTransform;
	mInstanceBufferData.clear();
	for (auto& transform : mpInstancedTransform->GetInstances())
		mInstanceBufferData.emplace_back(transform.GetWorld());
}

void Core::InstancedMVC::Initialize()
{
	Core::MeshModelVC::Initialize();
	Core::Application* pApplication{ mpObject->GetScene()->GetApplication() };
	auto pDevice{ pApplication->GetDevice() };
	{
		D3D12_HEAP_PROPERTIES heapProp{ CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD) };
		D3D12_RESOURCE_DESC resDesc{ CD3DX12_RESOURCE_DESC::Buffer(sizeof(Core::InstanceBuffer) * InstancedTransformMC::GetMaxInstances()) };
		mpObject->GetScene()->GetApplication()->ThrowIfFailed(pDevice->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mpInstanceBuffer)));

		CD3DX12_RANGE readRange(0, 0);
		mpObject->GetScene()->GetApplication()->ThrowIfFailed(mpInstanceBuffer->Map(0, &readRange, reinterpret_cast<void**>(&mpInstanceDataBegin)));
	}
	mIsInitialized = true;
}

void Core::InstancedMVC::Update(float delta)
{
	Core::MeshModelVC::Update(delta);
}

bool Core::InstancedMVC::Render(Core::Canvas* pCanvas, Core::Material3D* pMaterial, float distSq)
{
	if (!mIsInitialized)
	{
#ifdef ION_LOGGER
		mpObject->GetScene()->GetApplication()->GetServiceLocator().GetLogger()->Message(typeid(this).name(), Core::MsgType::Fatal, "InstancedModelVC.Render() while mIsInitialized == false");
#endif
		return false;
	}
	if (!Core::ViewC::Render(pCanvas, pMaterial, distSq))
		return false;
	if (!mIsActive)
		return false;
	if (mInstanceBufferData.empty())
		return false;
	if (mpTextures.empty())
		return false;

        auto pGraphicsCommandList{ pCanvas->GetGraphicsCommandList() };
        auto pApplication{ mpObject->GetScene()->GetApplication() };
        if (!pGraphicsCommandList || !mpObjectCbvDataBegin)
        {
#ifdef ION_LOGGER
                mpObject->GetScene()->GetApplication()->GetServiceLocator().GetLogger()->Message(
                        typeid(this).name(), Core::MsgType::Fatal,
                        "InstancedMVC.Render() invalid command list or descriptor heap");
#endif
                return false;
        }

        auto pDevice{ mpObject->GetScene()->GetApplication()->GetDevice() };

        // Write canvas constant buffer view into local heap
        D3D12_CONSTANT_BUFFER_VIEW_DESC canvasCbv{};
        canvasCbv.BufferLocation = pCanvas->GetCanvasConstantBuffer()->GetGPUVirtualAddress();
        canvasCbv.SizeInBytes = sizeof(Core::CanvasConstantBuffer);
        pDevice->CreateConstantBufferView(&canvasCbv, pApplication->GetCpuHandle(mCbvSrvOffset));

        // Bind canvas constant buffer descriptor table
        pGraphicsCommandList->SetGraphicsRootDescriptorTable(0, pApplication->GetGpuHandle(mCbvSrvOffset));

        UINT dsTable{ 1 };
        SetDescTableObjectConstants(pCanvas, dsTable);
        SetDescTableTextures(pCanvas, dsTable);
	if (!mpInstancedTransform->GetIsStatic())
		SetInstancedTransform(mpInstancedTransform);
	memcpy(mpInstanceDataBegin, mInstanceBufferData.data(), sizeof(Core::InstanceBuffer) * mInstanceBufferData.size());
	pGraphicsCommandList->SetGraphicsRootShaderResourceView(dsTable, mpInstanceBuffer->GetGPUVirtualAddress());

	pGraphicsCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pGraphicsCommandList->IASetIndexBuffer(&mIndexBufferView);
	pGraphicsCommandList->IASetVertexBuffers(0, 1, &mVertexBufferView);
	pGraphicsCommandList->DrawIndexedInstanced(UINT(mIndexCount), UINT(mInstanceBufferData.size()), 0, 0, 0);
	return true;
}
