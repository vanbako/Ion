#include "../Core/pch.h"
#include "../Core/InstancedModelVC.h"
#include "../Core/Application.h"
#include "../Core/Material.h"
#include "../Core/Object.h"
#include "../Core/Scene.h"
#include "../Core/CameraRMC.h"
#include "../Core/Model.h"
#include "../Core/d3dx12.h"

using namespace Ion::Core;

const size_t InstancedModelVC::mMaxInstances{ 40960 };

InstancedModelVC::InstancedModelVC(const std::string& modelName, const std::string& materialName, bool isActive, Winding winding, Object* pObject)
	: ViewC(isActive, pObject)
	, mpModel{ pObject->GetScene()->GetApplication()->AddModel(modelName, winding) }
	, mpMaterial{ pObject->GetScene()->GetApplication()->AddMaterial(materialName) }
	, mpTextures{}
	, mpVertices{ nullptr }
	, mIndexBuffer{}
	, mIndexBufferView{}
	, mpIndexDataBegin{ nullptr }
	, mIndexCount{ 0 }
	, mVertexCount{ 0 }
	, mTransforms{}
	, mVertexBuffer{}
	, mVertexBufferView{}
	, mpVertexDataBegin{ nullptr }
	, mpInstanceBuffer{}
	, mInstanceBufferData{}
	, mpInstanceDataBegin{ nullptr }
	, mpTextureSrvHeaps{}
	, mpCanvases{}
{
	mpMaterial->Initialize();
}

InstancedModelVC::~InstancedModelVC()
{
	if (mpVertices != nullptr)
		delete[] mpVertices;
}

void InstancedModelVC::AddCanvas(Canvas* pCanvas)
{
	mpMaterial->AddViewC(pCanvas, this);
	mpCanvases.emplace(pCanvas);
	pCanvas->AddMaterial(mpMaterial);
}

void InstancedModelVC::AddTexture(TextureType textureType, const std::string& name)
{
	if (mpTextures.contains(textureType))
		return;
	if (!mpMaterial->GetTextureTypeSet().contains(textureType))
		return;
	Application* pApplication{ mpObject->GetScene()->GetApplication() };
	auto pDevice{ pApplication->GetDevice() };
	Texture* pTexture{ mpObject->GetScene()->GetApplication()->AddTexture(name) };
	mpTextures[textureType] = pTexture;
	mpTextureSrvHeaps[textureType] = Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>{};
	{
		D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{};
		srvHeapDesc.NumDescriptors = 1;
		srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		ThrowIfFailed(pDevice->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&mpTextureSrvHeaps[textureType])));

		auto& texture{ pTexture->GetTexture() };
		CD3DX12_CPU_DESCRIPTOR_HANDLE hDesc{ mpTextureSrvHeaps[textureType]->GetCPUDescriptorHandleForHeapStart() };
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = texture->GetDesc().Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = texture->GetDesc().MipLevels;
		srvDesc.Texture2D.ResourceMinLODClamp = 0.f;
		pDevice->CreateShaderResourceView(texture.Get(), &srvDesc, hDesc);
	}
}

void InstancedModelVC::AddInstance(const TransformMC& transformMC)
{
	mTransforms.push_back(transformMC);
	mTransforms.back().Update(0.f);
	mInstanceBufferData.emplace_back(mTransforms.back().GetWorld());
}

void InstancedModelVC::AddInstances(const std::vector<TransformMC>& transformMCs)
{
	for (auto& transformMC : transformMCs)
		AddInstance(transformMC);
}

void InstancedModelVC::ReadInstances()
{
	if (!mTransforms.empty())
		return;
	const std::vector<Transform>& transforms{ mpModel->ReadInstances() };
	TransformMC transformMC{ true, mpObject };
	for (auto& transform : transforms)
	{
		transformMC.SetPosition(transform.mPosition);
		transformMC.SetScale(transform.mScale);
		transformMC.SetRotation(transform.mRotation);
		AddInstance(transformMC);
	}
}

std::vector<TransformMC>& InstancedModelVC::GetInstances()
{
	return mTransforms;
}

void InstancedModelVC::Initialize()
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
				std::memcpy(pPos, &mpModel->GetPositions()[i], sizeof(DirectX::XMFLOAT3));
				pPos += sizeof(DirectX::XMFLOAT3);
				break;
			case InputSemantic::Normal:
				std::memcpy(pPos, &mpModel->GetNormals()[i], sizeof(DirectX::XMFLOAT3));
				pPos += sizeof(DirectX::XMFLOAT3);
				break;
			case InputSemantic::Tangent:
				std::memcpy(pPos, &mpModel->GetTangents()[i], sizeof(DirectX::XMFLOAT3));
				pPos += sizeof(DirectX::XMFLOAT3);
				break;
			case InputSemantic::Binormal:
				std::memcpy(pPos, &mpModel->GetBinormals()[i], sizeof(DirectX::XMFLOAT3));
				pPos += sizeof(DirectX::XMFLOAT3);
				break;
			case InputSemantic::TexCoord:
				std::memcpy(pPos, &mpModel->GetTexCoords()[i], sizeof(DirectX::XMFLOAT2));
				pPos += sizeof(DirectX::XMFLOAT2);
				break;
			case InputSemantic::Color:
				std::memcpy(pPos, &mpModel->GetColors()[i], sizeof(DirectX::XMFLOAT4));
				pPos += sizeof(DirectX::XMFLOAT4);
				break;
			case InputSemantic::BlendIndices:
				std::memcpy(pPos, &mpModel->GetBlendIndices()[i], sizeof(DirectX::XMFLOAT4));
				pPos += sizeof(DirectX::XMFLOAT4);
				break;
			case InputSemantic::BlendWeight:
				std::memcpy(pPos, &mpModel->GetBlendWeights()[i], sizeof(DirectX::XMFLOAT4));
				pPos += sizeof(DirectX::XMFLOAT4);
				break;
			}
		}
	delete[] pInputSemantics;

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

		mVertexBufferView.BufferLocation = mVertexBuffer->GetGPUVirtualAddress();
		mVertexBufferView.StrideInBytes = UINT(layoutSize);
		mVertexBufferView.SizeInBytes = vertexBufferSize;
	}
	{
		D3D12_HEAP_PROPERTIES heapProp{ CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD) };
		D3D12_RESOURCE_DESC resDesc{ CD3DX12_RESOURCE_DESC::Buffer(sizeof(InstanceBuffer) * mMaxInstances) };
		ThrowIfFailed(pDevice->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mpInstanceBuffer)));

		CD3DX12_RANGE readRange(0, 0);
		ThrowIfFailed(mpInstanceBuffer->Map(0, &readRange, reinterpret_cast<void**>(&mpInstanceDataBegin)));
		memcpy(mpInstanceDataBegin, mInstanceBufferData.data(), sizeof(InstanceBuffer) * mInstanceBufferData.size());
	}

	// Test
	//for (int i{ 0 }; i < mMaxInstances; ++i)
	//{
	//	mTransforms.emplace_back(true, mpObject);
	//	mTransforms[i].SetPosition(DirectX::XMFLOAT4{ 5.f * float(i + 1), 0.f, 0.f, 0.f });
	//	mTransforms[i].Update(0.f);
	//	mInstanceBufferData.emplace_back(mTransforms[i].GetWorld());
	//}

	mIsInitialized = true;
}

void InstancedModelVC::Update(float delta)
{
	(delta);
	if (!mIsActive)
		return;
}

void InstancedModelVC::Render(Canvas* pCanvas, Material* pMaterial)
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

	UINT dsTable{ 1 };
	for (auto& pair : mpTextureSrvHeaps)
	{
		ID3D12DescriptorHeap* ppHeaps[]{ pair.second.Get() };
		pGraphicsCommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		CD3DX12_GPU_DESCRIPTOR_HANDLE tex{ pair.second->GetGPUDescriptorHandleForHeapStart() };
		pGraphicsCommandList->SetGraphicsRootDescriptorTable(dsTable, tex);
		++dsTable;
	}
	memcpy(mpInstanceDataBegin, mInstanceBufferData.data(), sizeof(InstanceBuffer) * mInstanceBufferData.size());
	pGraphicsCommandList->SetGraphicsRootShaderResourceView(dsTable, mpInstanceBuffer->GetGPUVirtualAddress());
	++dsTable;

	pGraphicsCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pGraphicsCommandList->IASetIndexBuffer(&mIndexBufferView);
	pGraphicsCommandList->IASetVertexBuffers(0, 1, &mVertexBufferView);
	pGraphicsCommandList->DrawIndexedInstanced(UINT(mIndexCount), UINT(mInstanceBufferData.size()), 0, 0, 0);
}
