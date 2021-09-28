#include "pch.h"
#include "MeshVC.h"
#include "Application.h"
#include "Material3D.h"
#include "Object.h"
#include "Scene.h"
#include "CameraRMC.h"

using namespace Ion;

Core::MeshVC::MeshVC(const std::string& materialName, bool isActive, Core::Object* pObject)
	: ViewC(isActive, pObject, materialName, "")
	, mMeshChanged{ false }
	, mVertices{}
	, mVertexCount{ 0 }
	, mVertexBuffer{}
	, mVertexBufferView{}
	, mpVertexDataBegin{ nullptr }
	, mpObjectCbvHeap{}
	, mpObjectConstantBuffer{}
	, mObjectConstantBufferData{}
	, mpObjectCbvDataBegin{ nullptr }
{
}

void Core::MeshVC::AddTriangle(const Core::VertexPNC& a, const Core::VertexPNC& b, const Core::VertexPNC& c)
{
	if (mVertexCount >= (mMaxVertices - 3))
		return;
	mVertices[mVertexCount] = a;
	mVertices[mVertexCount + 1] = b;
	mVertices[mVertexCount + 2] = c;
	mVertexCount += 3;
	mMeshChanged = true;
}

void Core::MeshVC::AddQuadrilateral(const Core::Quadrilateral<Core::VertexPNC>& quadrilateral)
{
	AddTriangle(quadrilateral.GetA(), quadrilateral.GetB(), quadrilateral.GetC());
	AddTriangle(quadrilateral.GetC(), quadrilateral.GetD(), quadrilateral.GetA());
}

void Core::MeshVC::Initialize()
{
	if (mIsInitialized)
		return;
	Core::Application* pApplication{ mpObject->GetScene()->GetApplication() };
	auto pDevice{ pApplication->GetDevice() };

	{
		D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc{};
		cbvHeapDesc.NumDescriptors = 1;
		cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		mpObject->GetScene()->GetApplication()->ThrowIfFailed(pDevice->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&mpObjectCbvHeap)));
	}

	{
		const UINT vertexBufferSize{ UINT(mVertexCount * sizeof(VertexPNC)) };
		D3D12_HEAP_PROPERTIES heapProp{ CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD) };
		D3D12_RESOURCE_DESC resDesc{ CD3DX12_RESOURCE_DESC::Buffer(mMaxVertices * sizeof(Core::VertexPNC)) };
		mpObject->GetScene()->GetApplication()->ThrowIfFailed(pDevice->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mVertexBuffer)));

		CD3DX12_RANGE readRange(0, 0);
		mpObject->GetScene()->GetApplication()->ThrowIfFailed(mVertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&mpVertexDataBegin)));
		memcpy(mpVertexDataBegin, mVertices, vertexBufferSize);
		//mVertexBuffer->Unmap(0, nullptr);

		mVertexBufferView.BufferLocation = mVertexBuffer->GetGPUVirtualAddress();
		mVertexBufferView.StrideInBytes = sizeof(Core::VertexPNC);
		mVertexBufferView.SizeInBytes = mMaxVertices * sizeof(Core::VertexPNC);
	}
	{
		const UINT objectConstantBufferSize{ sizeof(Core::MeshVCConstantBuffer) };

		D3D12_HEAP_PROPERTIES heapProp{ CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD) };
		D3D12_RESOURCE_DESC resDesc{ CD3DX12_RESOURCE_DESC::Buffer(objectConstantBufferSize) };
		mpObject->GetScene()->GetApplication()->ThrowIfFailed(pDevice->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mpObjectConstantBuffer)));

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
		cbvDesc.BufferLocation = mpObjectConstantBuffer->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = objectConstantBufferSize;
		pDevice->CreateConstantBufferView(&cbvDesc, mpObjectCbvHeap->GetCPUDescriptorHandleForHeapStart());

		CD3DX12_RANGE readRange(0, 0);
		mpObject->GetScene()->GetApplication()->ThrowIfFailed(mpObjectConstantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&mpObjectCbvDataBegin)));
		memcpy(mpObjectCbvDataBegin, &mObjectConstantBufferData, sizeof(mObjectConstantBufferData));
	}
	mIsInitialized = true;
}

void Core::MeshVC::Update(float delta)
{
	(delta);
	if (!mIsActive)
		return;
	if (mMeshChanged)
	{
		const UINT vertexBufferSize{ UINT(mVertexCount * sizeof(Core::VertexPNC)) };
		//CD3DX12_RANGE readRange(0, 0);
		//ThrowIfFailed(mVertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&mpVertexDataBegin)));
		memcpy(mpVertexDataBegin, mVertices, vertexBufferSize);
		//mVertexBuffer->Unmap(0, nullptr);
		mMeshChanged = false;
	}
}

bool Core::MeshVC::Render(Core::Canvas* pCanvas, Core::Material3D* pMaterial)
{
	if (!mIsInitialized)
	{
#ifdef ION_LOGGER
		mpObject->GetScene()->GetApplication()->GetServiceLocator().GetLogger()->Message(typeid(this).name(), Core::MsgType::Fatal, "MeshVC.Render() while mIsInitialized == false");
#endif
		return false;
	}
	(pMaterial);
	if (!Core::ViewC::Render(pCanvas, pMaterial))
		return false;
	if (!mIsActive)
		return false;
	if (mVertexCount == 0)
		return false;

	Core::Application* pApplication{ mpObject->GetScene()->GetApplication() };
	auto pDxgiFactory{ pApplication->GetDxgiFactory() };
	auto pDevice{ pApplication->GetDevice() };
	auto pCmdQueue{ pApplication->GetCommandQueue() };
	auto pGraphicsCommandList{ pCanvas->GetGraphicsCommandList() };

	DirectX::XMMATRIX world{ DirectX::XMLoadFloat4x4(&mpObject->GetModelC<Core::TransformMC>()->GetWorld()) };
	const DirectX::XMMATRIX viewProjection{ DirectX::XMLoadFloat4x4(&pCanvas->GetCamera()->GetModelC<Core::CameraRMC>()->GetViewProjection()) };
	DirectX::XMMATRIX wvp{ world * viewProjection };

	DirectX::XMStoreFloat4x4(&mObjectConstantBufferData.mWorld , world);
	DirectX::XMStoreFloat4x4(&mObjectConstantBufferData.mWorldViewProj, wvp);

	memcpy(mpObjectCbvDataBegin, &mObjectConstantBufferData, sizeof(mObjectConstantBufferData));
	{
		ID3D12DescriptorHeap* ppHeaps[]{ mpObjectCbvHeap.Get() };
		pGraphicsCommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		pGraphicsCommandList->SetGraphicsRootDescriptorTable(1, mpObjectCbvHeap->GetGPUDescriptorHandleForHeapStart());
	}

	pGraphicsCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pGraphicsCommandList->IASetVertexBuffers(0, 1, &mVertexBufferView);
	pGraphicsCommandList->DrawInstanced(UINT(mVertexCount), 1, 0, 0);
	return true;
}
