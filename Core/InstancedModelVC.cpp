#include "pch.h"
#include "InstancedModelVC.h"
#include "Application.h"
#include "Material3D.h"
#include "Object.h"
#include "Scene.h"
#include "CameraRMC.h"
#include "Model.h"
#include "Canvas.h"
#include "d3dx12.h"

using namespace Ion;

const size_t Core::InstancedModelVC::mMaxInstances{ 40960 };

Core::InstancedModelVC::InstancedModelVC(const std::string& modelName, const std::string& modelExtension, const std::string& materialName, bool isActive, Core::Winding winding, Core::CoordSystem coordSystem, Core::Object* pObject)
	: ModelVC(modelName, modelExtension, materialName, isActive, winding, coordSystem, pObject)
	, mTransforms{}
	, mpInstanceBuffer{}
	, mInstanceBufferData{}
	, mpInstanceDataBegin{ nullptr }
{
}

Core::InstancedModelVC::~InstancedModelVC()
{
}

void Core::InstancedModelVC::AddInstance(const Core::TransformMC& transformMC)
{
	mTransforms.push_back(transformMC);
	mTransforms.back().Update(0.f);
	mInstanceBufferData.emplace_back(mTransforms.back().GetWorld());
}

void Core::InstancedModelVC::AddInstances(const std::vector<Core::TransformMC>& transformMCs)
{
	for (auto& transformMC : transformMCs)
		AddInstance(transformMC);
}

void Core::InstancedModelVC::ReadInstances()
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

std::vector<Core::TransformMC>& Core::InstancedModelVC::GetInstances()
{
	return mTransforms;
}

void Core::InstancedModelVC::Initialize()
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
	mIsInitialized = true;
}

void Core::InstancedModelVC::Update(float delta)
{
	if (!mIsActive)
		return;
	Core::ModelVC::Update(delta);
}

void Core::InstancedModelVC::Render(Core::Canvas* pCanvas, Core::Material3D* pMaterial)
{
	(pMaterial);
	if (!mIsActive)
		return;

	Core::Application* pApplication{ mpObject->GetScene()->GetApplication() };
	auto pDxgiFactory{ pApplication->GetDxgiFactory() };
	auto pDevice{ pApplication->GetDevice() };
	auto pCmdQueue{ pApplication->GetCommandQueue() };
	auto pCmdAlloc{ pApplication->GetCommandAllocator() };
	auto pGraphicsCommandList{ pCanvas->GetGraphicsCommandList() };

	UINT dsTable{ 1 };
	for (auto& pair : mpTextureSrvHeaps)
	{
		ID3D12DescriptorHeap* ppHeaps[]{ pair.second.Get() };
		pGraphicsCommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		CD3DX12_GPU_DESCRIPTOR_HANDLE tex{ pair.second->GetGPUDescriptorHandleForHeapStart() };
		pGraphicsCommandList->SetGraphicsRootDescriptorTable(dsTable, tex);
		++dsTable;
	}
	memcpy(mpInstanceDataBegin, mInstanceBufferData.data(), sizeof(Core::InstanceBuffer) * mInstanceBufferData.size());
	pGraphicsCommandList->SetGraphicsRootShaderResourceView(dsTable, mpInstanceBuffer->GetGPUVirtualAddress());
	++dsTable;

	pGraphicsCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pGraphicsCommandList->IASetIndexBuffer(&mIndexBufferView);
	pGraphicsCommandList->IASetVertexBuffers(0, 1, &mVertexBufferView);
	pGraphicsCommandList->DrawIndexedInstanced(UINT(mIndexCount), UINT(mInstanceBufferData.size()), 0, 0, 0);
}
