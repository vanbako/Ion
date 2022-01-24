#include "pch.h"
#include "MeshModelVC.h"
#include "Application.h"
#include "Material3D.h"
#include "Object.h"
#include "Scene.h"
#include "CameraRMC.h"
#include "Model.h"
#include "Canvas.h"
#include "InstancedTransformMC.h"
#include "MeshModel.h"
#include "MeshModelResource.h"
#include "TextureResource.h"
#include "Texture.h"
#include "Model.h"

using namespace Ion;

Core::MeshModelVC::MeshModelVC(const std::string& modelName, const std::string& modelExtension, const std::string& materialName, bool isActive, Core::Winding winding, Core::CoordSystem coordSystem, Core::Object* pObject)
	: Core::ViewC(isActive, pObject, materialName, "")
	, mName{ modelName }
	, mpMeshModel{ pObject->GetScene()->GetApplication()->GetResourceManager()->GetResource<MeshModelResource>()->AddMeshModel(modelName, modelExtension, winding, coordSystem) }
	, mTextureNames{}
	, mpTextures{}
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
	, mpTextureSrvHeaps{}
{
	mObjectConstantBufferData.mShininess = 20.f;
}

Core::MeshModelVC::~MeshModelVC()
{
	if (mpVertices != nullptr)
		delete[] mpVertices;
	Core::ResourceManager* pResourceManager{ mpObject->GetScene()->GetApplication()->GetResourceManager() };
	pResourceManager->GetResource<MeshModelResource>()->RemoveModel(mName);
	for (std::string& name : mTextureNames)
		pResourceManager->GetResource<TextureResource>()->RemoveTexture(name);
}

void Core::MeshModelVC::AddTexture(Core::TextureType textureType, const std::string& name)
{
	if (mpTextures.contains(textureType))
		return;
	if (!mpMaterial3D->GetTextureTypeSet().contains(textureType))
		return;
	Core::Application* pApplication{ mpObject->GetScene()->GetApplication() };
	auto pDevice{ pApplication->GetDevice() };
	mTextureNames.emplace_back(name);
	Core::Texture* pTexture{ mpObject->GetScene()->GetApplication()->GetResourceManager()->GetResource<TextureResource>()->AddTexture(name) };
	mpTextures[textureType] = pTexture;
	mpTextureSrvHeaps[textureType] = Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>{};
	{
		D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{};
		srvHeapDesc.NumDescriptors = 1;
		srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		mpObject->GetScene()->GetApplication()->ThrowIfFailed(pDevice->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&mpTextureSrvHeaps[textureType])));

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

void Core::MeshModelVC::SetShininess(float shininess)
{
	mObjectConstantBufferData.mShininess = shininess;
}

void Core::MeshModelVC::Initialize()
{
	Core::Application* pApplication{ mpObject->GetScene()->GetApplication() };
	auto pDevice{ pApplication->GetDevice() };

	// Build vertex buffer
	D3D12_INPUT_ELEMENT_DESC* pInputElementDescs{ mpMaterial3D->GetInputElementDescs() };
	UINT inputElementCount{ mpMaterial3D->GetInputElementCount() };
	for (UINT i{ 0 }; i < inputElementCount; ++i)
		if (!mpMeshModel->HasInputElem(pInputElementDescs[i].SemanticName))
			return;
	std::size_t layoutSize{ mpMaterial3D->GetLayoutSize() };
	mVertexCount = mpMeshModel->GetPositions().size();
	mpVertices = new char[mVertexCount * layoutSize];
	char* pPos{ mpVertices };
	Core::InputSemantic* pInputSemantics{ new Core::InputSemantic[inputElementCount]{} };
	for (UINT j{ 0 }; j < inputElementCount; ++j)
		pInputSemantics[j] = Core::Material3D::GetSemanticStrings().at(pInputElementDescs[j].SemanticName).inputSemantic;
	for (std::size_t i{ 0 }; i < mVertexCount; ++i)
		for (UINT j{ 0 }; j < inputElementCount; ++j)
		{
			switch (pInputSemantics[j])
			{
			case Core::InputSemantic::Position:
				std::memcpy(pPos, &mpMeshModel->GetPositions()[i], sizeof(DirectX::XMFLOAT3));
				//*((DirectX::XMFLOAT3*)pPos) = mpModel->GetPositions()[i];
				pPos += sizeof(DirectX::XMFLOAT3);
				break;
			case Core::InputSemantic::Normal:
				std::memcpy(pPos, &mpMeshModel->GetNormals()[i], sizeof(DirectX::XMFLOAT3));
				//*((DirectX::XMFLOAT3*)pPos) = mpModel->GetNormals()[i];
				pPos += sizeof(DirectX::XMFLOAT3);
				break;
			case Core::InputSemantic::Tangent:
				std::memcpy(pPos, &mpMeshModel->GetTangents()[i], sizeof(DirectX::XMFLOAT3));
				//*((DirectX::XMFLOAT3*)pPos) = mpModel->GetTangents()[i];
				pPos += sizeof(DirectX::XMFLOAT3);
				break;
			case Core::InputSemantic::Binormal:
				std::memcpy(pPos, &mpMeshModel->GetBinormals()[i], sizeof(DirectX::XMFLOAT3));
				//*((DirectX::XMFLOAT3*)pPos) = mpModel->GetBinormals()[i];
				pPos += sizeof(DirectX::XMFLOAT3);
				break;
			case Core::InputSemantic::TexCoord:
				std::memcpy(pPos, &mpMeshModel->GetTexCoords()[i], sizeof(DirectX::XMFLOAT2));
				//*((DirectX::XMFLOAT2*)pPos) = mpModel->GetTexCoords()[i];
				pPos += sizeof(DirectX::XMFLOAT2);
				break;
			case Core::InputSemantic::Color:
				std::memcpy(pPos, &mpMeshModel->GetColors()[i], sizeof(DirectX::XMFLOAT4));
				//*((DirectX::XMFLOAT4*)pPos) = mpModel->GetColors()[i];
				pPos += sizeof(DirectX::XMFLOAT4);
				break;
			case Core::InputSemantic::BlendIndices:
				std::memcpy(pPos, &mpMeshModel->GetBlendIndices()[i], sizeof(DirectX::XMFLOAT4));
				//*((DirectX::XMFLOAT4*)pPos) = mpModel->GetBlendIndices()[i];
				pPos += sizeof(DirectX::XMFLOAT4);
				break;
			case Core::InputSemantic::BlendWeight:
				std::memcpy(pPos, &mpMeshModel->GetBlendWeights()[i], sizeof(DirectX::XMFLOAT4));
				//*((DirectX::XMFLOAT4*)pPos) = mpModel->GetBlendWeights()[i];
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
		mpObject->GetScene()->GetApplication()->ThrowIfFailed(pDevice->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&mpObjectCbvHeap)));
	}

	mIndexCount = mpMeshModel->GetIndices().size();
	{
		const UINT indexBufferSize{ UINT(mIndexCount * sizeof(DWORD)) };
		D3D12_HEAP_PROPERTIES heapProp{ CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD) };
		D3D12_RESOURCE_DESC resDesc{ CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize) };
		mpObject->GetScene()->GetApplication()->ThrowIfFailed(pDevice->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mIndexBuffer)));

		CD3DX12_RANGE readRange(0, 0);
		mpObject->GetScene()->GetApplication()->ThrowIfFailed(mIndexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&mpIndexDataBegin)));
		memcpy(mpIndexDataBegin, mpMeshModel->GetIndices().data(), indexBufferSize);

		mIndexBufferView.BufferLocation = mIndexBuffer->GetGPUVirtualAddress();
		mIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
		mIndexBufferView.SizeInBytes = indexBufferSize;
	}
	{
		const UINT vertexBufferSize{ UINT(mVertexCount * layoutSize) };
		D3D12_HEAP_PROPERTIES heapProp{ CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD) };
		D3D12_RESOURCE_DESC resDesc{ CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize) };
		mpObject->GetScene()->GetApplication()->ThrowIfFailed(pDevice->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mVertexBuffer)));

		CD3DX12_RANGE readRange(0, 0);
		mpObject->GetScene()->GetApplication()->ThrowIfFailed(mVertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&mpVertexDataBegin)));
		memcpy(mpVertexDataBegin, mpVertices, vertexBufferSize);

		mVertexBufferView.BufferLocation = mVertexBuffer->GetGPUVirtualAddress();
		mVertexBufferView.StrideInBytes = UINT(layoutSize);
		mVertexBufferView.SizeInBytes = vertexBufferSize;
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

void Core::MeshModelVC::Update(float delta)
{
	(delta);
	//if (!mIsActive)
	//	return;
}

bool Core::MeshModelVC::Render(Core::Canvas* pCanvas, Core::Material3D* pMaterial)
{
	if (!mIsInitialized)
	{
#ifdef ION_LOGGER
		mpObject->GetScene()->GetApplication()->GetServiceLocator().GetLogger()->Message(typeid(this).name(), Core::MsgType::Fatal, "ModelVC.Render() while mIsInitialized == false");
#endif
		return false;
	}
	if (!Core::ViewC::Render(pCanvas, pMaterial))
		return false;
	if (!mIsActive)
		return false;

	auto pGraphicsCommandList{ pCanvas->GetGraphicsCommandList() };

	UINT dsTable{ 1 };
	SetDescTableObjectConstants(pCanvas, dsTable);
	SetDescTableTextures(pCanvas, dsTable);
	pGraphicsCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pGraphicsCommandList->IASetIndexBuffer(&mIndexBufferView);
	pGraphicsCommandList->IASetVertexBuffers(0, 1, &mVertexBufferView);
	pGraphicsCommandList->DrawIndexedInstanced(UINT(mIndexCount), 1, 0, 0, 0);
	return true;
}

void Core::MeshModelVC::SetDescTableObjectConstants(Core::Canvas* pCanvas, UINT& dsTable)
{
	auto pGraphicsCommandList{ pCanvas->GetGraphicsCommandList() };

	DirectX::XMMATRIX world{ DirectX::XMMatrixIdentity() };
	Core::InstancedTransformMC* pInstancedTransformMC{ mpObject->GetModelC<InstancedTransformMC>() };
	Core::TransformMC* pTransformMC{ mpObject->GetModelC<TransformMC>() };
	if (pInstancedTransformMC == nullptr)
		world = DirectX::XMLoadFloat4x4(&pTransformMC->GetWorld());
	else
		if (pInstancedTransformMC->GetIsStatic())
			world = DirectX::XMLoadFloat4x4(&pTransformMC->GetWorld());
	const DirectX::XMMATRIX viewProjection{ DirectX::XMLoadFloat4x4(&pCanvas->GetCamera()->GetModelC<CameraRMC>()->GetViewProjection()) };
	DirectX::XMMATRIX wvp{ world * viewProjection };

	DirectX::XMStoreFloat4x4(&mObjectConstantBufferData.mWorld, world);
	DirectX::XMStoreFloat4x4(&mObjectConstantBufferData.mWorldViewProj, wvp);

	memcpy(mpObjectCbvDataBegin, &mObjectConstantBufferData, sizeof(mObjectConstantBufferData));
	{
		ID3D12DescriptorHeap* ppHeaps[]{ mpObjectCbvHeap.Get() };
		pGraphicsCommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		pGraphicsCommandList->SetGraphicsRootDescriptorTable(dsTable, mpObjectCbvHeap->GetGPUDescriptorHandleForHeapStart());
	}
	++dsTable;
}

void Core::MeshModelVC::SetDescTableTextures(Core::Canvas* pCanvas, UINT& dsTable)
{
	auto pGraphicsCommandList{ pCanvas->GetGraphicsCommandList() };

	for (auto& pair : mpTextureSrvHeaps)
	{
		ID3D12DescriptorHeap* ppHeaps[]{ pair.second.Get() };
		pGraphicsCommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		CD3DX12_GPU_DESCRIPTOR_HANDLE tex{ pair.second->GetGPUDescriptorHandleForHeapStart() };
		pGraphicsCommandList->SetGraphicsRootDescriptorTable(dsTable, tex);
		++dsTable;
	}
}
