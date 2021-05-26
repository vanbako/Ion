#include "../Core/pch.h"
#include "../Core/MeshVC.h"
#include "../Core/Application.h"
#include "../Core/Helpers.h"
#include "../Core/Material.h"
#include "../Core/Object.h"
#include "../Core/Scene.h"
#include "../Core/CameraMC.h"
#include "../Core/d3dx12.h"

using namespace Ion::Core;

MeshVC::MeshVC(bool isActive, Object* pObject)
	: ViewC(isActive, pObject)
	, mMeshChanged{ false }
	, mVertices{}
	, mVertexCount{ 0 }
	, mpMaterial{ pObject->GetScene()->GetApplication()->AddMaterial("PosNormCol") }
	, mVertexBuffer{}
	, mVertexBufferView{}
	, mpVertexDataBegin{ nullptr }
	, mpObjectCbvHeap{}
	, mpObjectConstantBuffer{}
	, mObjectConstantBufferData{}
	, mpObjectCbvDataBegin{ nullptr }
	, mpCanvases{}
{
	mpMaterial->AddViewC(this);
	D3D12_INPUT_ELEMENT_DESC* inputElementDescs{ new D3D12_INPUT_ELEMENT_DESC[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	} };
	mpMaterial->SetInputLayout(inputElementDescs, 3);
	mpMaterial->Initialize();
}

MeshVC::~MeshVC()
{
}

void MeshVC::AddCanvas(Canvas* pCanvas)
{
	mpCanvases.emplace(pCanvas);
	pCanvas->AddMaterial(mpMaterial);
}

void MeshVC::AddTriangle(const VertexPNC& a, const VertexPNC& b, const VertexPNC& c)
{
	if (mVertexCount >= (mMaxVertices - 3))
		return;
	mVertices[mVertexCount] = a;
	mVertices[mVertexCount + 1] = b;
	mVertices[mVertexCount + 2] = c;
	mVertexCount += 3;
	mMeshChanged = true;
}

void MeshVC::AddQuadrilateral(const Quadrilateral<VertexPNC>& quadrilateral)
{
	AddTriangle(quadrilateral.GetA(), quadrilateral.GetB(), quadrilateral.GetC());
	AddTriangle(quadrilateral.GetC(), quadrilateral.GetD(), quadrilateral.GetA());
}

void MeshVC::Initialize()
{
	Application* pApplication{ mpObject->GetScene()->GetApplication() };
	auto pDevice{ pApplication->GetDevice() };

	{
		D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc{};
		cbvHeapDesc.NumDescriptors = 1;
		cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		ThrowIfFailed(pDevice->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&mpObjectCbvHeap)));
	}

	{
		const UINT vertexBufferSize{ UINT(mVertexCount * sizeof(VertexPNC)) };
		D3D12_HEAP_PROPERTIES heapProp{ CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD) };
		D3D12_RESOURCE_DESC resDesc{ CD3DX12_RESOURCE_DESC::Buffer(mMaxVertices * sizeof(VertexPNC)) };
		ThrowIfFailed(pDevice->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mVertexBuffer)));

		CD3DX12_RANGE readRange(0, 0);
		ThrowIfFailed(mVertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&mpVertexDataBegin)));
		memcpy(mpVertexDataBegin, mVertices, vertexBufferSize);
		//mVertexBuffer->Unmap(0, nullptr);

		mVertexBufferView.BufferLocation = mVertexBuffer->GetGPUVirtualAddress();
		mVertexBufferView.StrideInBytes = sizeof(VertexPNC);
		mVertexBufferView.SizeInBytes = mMaxVertices * sizeof(VertexPNC);
	}
	{
		const UINT objectConstantBufferSize{ sizeof(MeshVCConstantBuffer) };

		D3D12_HEAP_PROPERTIES heapProp{ CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD) };
		D3D12_RESOURCE_DESC resDesc{ CD3DX12_RESOURCE_DESC::Buffer(objectConstantBufferSize) };
		ThrowIfFailed(pDevice->CreateCommittedResource(
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
		ThrowIfFailed(mpObjectConstantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&mpObjectCbvDataBegin)));
		memcpy(mpObjectCbvDataBegin, &mObjectConstantBufferData, sizeof(mObjectConstantBufferData));
	}
	mIsInitialized = true;
}

void MeshVC::Update(float delta)
{
	(delta);
	if (!mIsActive)
		return;
	if (mMeshChanged)
	{
		const UINT vertexBufferSize{ UINT(mVertexCount * sizeof(VertexPNC)) };
		//CD3DX12_RANGE readRange(0, 0);
		//ThrowIfFailed(mVertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&mpVertexDataBegin)));
		memcpy(mpVertexDataBegin, mVertices, vertexBufferSize);
		//mVertexBuffer->Unmap(0, nullptr);
		mMeshChanged = false;
	}
}

void MeshVC::Render(Canvas* pCanvas, Material* pMaterial)
{
	(pMaterial);
	if (!mIsActive)
		return;
	if (mVertexCount == 0)
		return;

	Application* pApplication{ mpObject->GetScene()->GetApplication() };
	auto pDxgiFactory{ pApplication->GetDxgiFactory() };
	auto pDevice{ pApplication->GetDevice() };
	auto pCmdQueue{ pApplication->GetCommandQueue() };
	auto pCmdAlloc{ pApplication->GetCommandAllocator() };
	auto pGraphicsCommandList{ pCanvas->GetGraphicsCommandList() };

	DirectX::XMMATRIX world{ DirectX::XMLoadFloat4x4(&mpObject->GetModelC<TransformMC>()->GetWorld()) };
	const DirectX::XMMATRIX viewProjection{ DirectX::XMLoadFloat4x4(&pCanvas->GetCamera()->GetModelC<CameraMC>()->GetViewProjection()) };
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
}
