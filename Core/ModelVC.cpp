#include "../Core/pch.h"
#include "../Core/ModelVC.h"
#include "../Core/Application.h"
#include "../Core/Material.h"
#include "../Core/Object.h"
#include "../Core/Scene.h"
#include "../Core/CameraRMC.h"
#include "../Core/Model.h"
#include "../Core/d3dx12.h"

using namespace Ion::Core;

ModelVC::ModelVC(const std::string& modelName, const std::string& materialName, bool isActive, Object* pObject)
	: ViewC(isActive, pObject)
	, mpModel{ pObject->GetScene()->GetApplication()->AddModel(modelName) }
	, mpMaterial{ pObject->GetScene()->GetApplication()->AddMaterial(materialName) }
	, mpVertices{ nullptr }
	, mIndexBuffer{}
	, mIndexBufferView{}
	, mpIndexDataBegin{ nullptr }
	, mIndexCount{ 0 }
	, mVertexCount{ 0 }
	, mVertexBuffer{}
	, mVertexBufferView{}
	, mpVertexDataBegin{ nullptr }
	, mpObjectCbvHeap{}
	, mpObjectConstantBuffer{}
	, mObjectConstantBufferData{}
	, mpObjectCbvDataBegin{ nullptr }
	, mpCanvases{}
{
	mpMaterial->Initialize();
}

ModelVC::~ModelVC()
{
	if (mpVertices != nullptr)
		delete[] mpVertices;
}

void ModelVC::AddCanvas(Canvas* pCanvas)
{
	mpMaterial->AddViewC(pCanvas, this);
	mpCanvases.emplace(pCanvas);
	pCanvas->AddMaterial(mpMaterial);
}

void ModelVC::Initialize()
{
	Application* pApplication{ mpObject->GetScene()->GetApplication() };
	auto pDevice{ pApplication->GetDevice() };

	// Build vertex buffer
	D3D12_INPUT_ELEMENT_DESC* pInputElementDescs{ mpMaterial->GetInputElementDescs() };
	UINT inputElementCount{ mpMaterial->GetInputElementCount() };
	for (UINT i{ 0 }; i < inputElementCount; ++i)
		if (!mpModel->HasInputElem(pInputElementDescs[i].SemanticName))
			return;
	size_t layoutSize{ mpMaterial->GetLayoutSize() };
	mVertexCount = mpModel->GetPositions().size();
	mpVertices = new char[mVertexCount * layoutSize];
	char* pPos{ mpVertices };
	InputSemantic* pInputSemantics{ new InputSemantic[inputElementCount]{} };
	for (UINT j{ 0 }; j < inputElementCount; ++j)
		pInputSemantics[j] = Material::GetSemanticStrings().at(pInputElementDescs[j].SemanticName).inputSemantic;
	for (size_t i{ 0 }; i < mVertexCount; ++i)
		for (UINT j{ 0 }; j < inputElementCount; ++j)
		{
			switch (pInputSemantics[j])
			{
			case InputSemantic::Position:
				*((DirectX::XMFLOAT3*)pPos) = mpModel->GetPositions()[i];
				pPos += sizeof(DirectX::XMFLOAT3);
				break;
			case InputSemantic::Normal:
				*((DirectX::XMFLOAT3*)pPos) = mpModel->GetNormals()[i];
				pPos += sizeof(DirectX::XMFLOAT3);
				break;
			case InputSemantic::Tangent:
				*((DirectX::XMFLOAT3*)pPos) = mpModel->GetTangents()[i];
				pPos += sizeof(DirectX::XMFLOAT3);
				break;
			case InputSemantic::Binormal:
				*((DirectX::XMFLOAT3*)pPos) = mpModel->GetBinormals()[i];
				pPos += sizeof(DirectX::XMFLOAT3);
				break;
			case InputSemantic::TexCoord:
				*((DirectX::XMFLOAT2*)pPos) = mpModel->GetTexCoords()[i];
				pPos += sizeof(DirectX::XMFLOAT2);
				break;
			case InputSemantic::Color:
				*((DirectX::XMFLOAT4*)pPos) = mpModel->GetColors()[i];
				pPos += sizeof(DirectX::XMFLOAT4);
				break;
			case InputSemantic::BlendIndices:
				*((DirectX::XMFLOAT4*)pPos) = mpModel->GetBlendIndices()[i];
				pPos += sizeof(DirectX::XMFLOAT4);
				break;
			case InputSemantic::BlendWeight:
				*((DirectX::XMFLOAT4*)pPos) = mpModel->GetBlendWeights()[i];
				pPos += sizeof(DirectX::XMFLOAT4);
				break;
			}
		}
	delete[] pInputSemantics;

	{
		D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc{};
		cbvHeapDesc.NumDescriptors = 1;
		cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		ThrowIfFailed(pDevice->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&mpObjectCbvHeap)));
	}

	mIndexCount = mpModel->GetIndices().size();
	{
		const UINT indexBufferSize{ UINT(mIndexCount * sizeof(DWORD)) };
		D3D12_HEAP_PROPERTIES heapProp{ CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD) };
		D3D12_RESOURCE_DESC resDesc{ CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize) };
		ThrowIfFailed(pDevice->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mIndexBuffer)));

		CD3DX12_RANGE readRange(0, 0);
		ThrowIfFailed(mIndexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&mpIndexDataBegin)));
		memcpy(mpIndexDataBegin, mpModel->GetIndices().data(), indexBufferSize);
		//mVertexBuffer->Unmap(0, nullptr);

		mIndexBufferView.BufferLocation = mIndexBuffer->GetGPUVirtualAddress();
		mIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
		mIndexBufferView.SizeInBytes = indexBufferSize;
	}
	{
		const UINT vertexBufferSize{ UINT(mVertexCount * layoutSize) };
		D3D12_HEAP_PROPERTIES heapProp{ CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD) };
		D3D12_RESOURCE_DESC resDesc{ CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize) };
		ThrowIfFailed(pDevice->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mVertexBuffer)));

		CD3DX12_RANGE readRange(0, 0);
		ThrowIfFailed(mVertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&mpVertexDataBegin)));
		memcpy(mpVertexDataBegin, mpVertices, vertexBufferSize);
		//mVertexBuffer->Unmap(0, nullptr);

		mVertexBufferView.BufferLocation = mVertexBuffer->GetGPUVirtualAddress();
		mVertexBufferView.StrideInBytes = UINT(layoutSize);
		mVertexBufferView.SizeInBytes = vertexBufferSize;
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

void ModelVC::Update(float delta)
{
	(delta);
	//if (!mIsActive)
	//	return;
}

void ModelVC::Render(Canvas* pCanvas, Material* pMaterial)
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

	DirectX::XMStoreFloat4x4(&mObjectConstantBufferData.mWorld , world);
	DirectX::XMStoreFloat4x4(&mObjectConstantBufferData.mWorldViewProj, wvp);

	memcpy(mpObjectCbvDataBegin, &mObjectConstantBufferData, sizeof(mObjectConstantBufferData));
	{
		ID3D12DescriptorHeap* ppHeaps[]{ mpObjectCbvHeap.Get() };
		pGraphicsCommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		pGraphicsCommandList->SetGraphicsRootDescriptorTable(1, mpObjectCbvHeap->GetGPUDescriptorHandleForHeapStart());
	}

	pGraphicsCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pGraphicsCommandList->IASetIndexBuffer(&mIndexBufferView);
	pGraphicsCommandList->IASetVertexBuffers(0, 1, &mVertexBufferView);
	pGraphicsCommandList->DrawIndexedInstanced(UINT(mIndexCount), 1, 0, 0, 0);
}