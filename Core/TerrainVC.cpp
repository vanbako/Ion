#include "pch.h"
#include "TerrainVC.h"
#include "Application.h"
#include "Material3D.h"
#include "Object.h"
#include "Scene.h"
#include "CameraRMC.h"
#include "TextureResource.h"
#include "Texture.h"
#include <algorithm>

using namespace Ion;

Core::TerrainVC::TerrainVC(const std::string& filename, float width, float depth, std::size_t rowCount, std::size_t colCount, bool isActive, Core::Object* pObject)
	: ViewC(isActive, pObject, "Terrain_A", "")
	, mFileName{ filename }
	, mScale{ width / float(rowCount), 128.f, depth / float(colCount) }
	, mWidth{ width }
	, mDepth{ depth }
	, mRowCount{ rowCount }
	, mColCount{ colCount }
	, mIndices{}
	, mIndexBuffer{}
	, mIndexBufferView{}
	, mpIndexDataBegin{ nullptr }
	, mVertices{}
	, mVertexBuffer{}
	, mVertexBufferView{}
	, mpVertexDataBegin{ nullptr }
	, mpObjectCbvHeap{}
	, mpObjectConstantBuffer{}
	, mObjectConstantBufferData{}
	, mpObjectCbvDataBegin{ nullptr }
        , mTextureNames{}
        , mTextureTypeOrder{}
        , mpTextures{}
        , mpTextureSrvHeaps{}
       , mCbvSrvOffset{ 0 }
       , mCbvSrvDescriptorSize{ 0 }
       , mTextureOffsets{}
{
	mObjectConstantBufferData.mShininess = 40.f;
}

Core::TerrainVC::~TerrainVC()
{
	Core::Application* pApplication{ mpObject->GetScene()->GetApplication() };
	for (std::string& name : mTextureNames)
		pApplication->GetResourceManager()->GetResource<TextureResource>()->RemoveTexture(name);
}

void Core::TerrainVC::AddTexture(Core::TextureType textureType, const std::string& name)
{
	if (mpTextures.contains(textureType))
		return;
	if (!mpMaterial3D->GetTextureTypeSet().contains(textureType))
		return;
	Core::Application* pApplication{ mpObject->GetScene()->GetApplication() };
	auto pDevice{ pApplication->GetDevice() };
       mTextureNames.emplace_back(name);
       mTextureTypeOrder.push_back(textureType);
       Core::Texture* pTexture{ mpObject->GetScene()->GetApplication()->GetResourceManager()->GetResource<TextureResource>()->AddTexture(name)};
	mpTextures[textureType] = pTexture;
	mpTextureSrvHeaps[textureType] = Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>{};
	{
               D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{};
               srvHeapDesc.NumDescriptors = 1;
               srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
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

float Core::TerrainVC::GetHeight(const DirectX::XMFLOAT2& xz)
{
	long long
		x{ long long((xz.x + mWidth / 2.f) / mScale.x) },
		z{ long long((xz.y + mDepth / 2.f) / mScale.z) };
	if ((x < 0) || (x >= long long(mRowCount)) || (z < 0) || (z >= long long(mColCount)))
		return 0.f;
	return float(mHeights[x * mColCount + z]) / 65536.f * mScale.y;
}

void Core::TerrainVC::SetShininess(float shininess)
{
	mObjectConstantBufferData.mShininess = shininess;
}

void Core::TerrainVC::Initialize()
{
	using namespace DirectX;

	if (mIsInitialized)
		return;
	Core::Application* pApplication{ mpObject->GetScene()->GetApplication() };
	auto pDevice{ pApplication->GetDevice() };

	std::size_t vertexCount{ mRowCount * mColCount };
	// TODO: Add enum for signed / unsigned heightmap
	// TODO: Add parameter for y-scale

	// Heights
	mHeights.resize(vertexCount, 0);
	std::ifstream heightsFile{ "../Resources/Terrain/" + mFileName, std::ios_base::binary};
#ifdef _DEBUG
	if (!heightsFile)
	{
#ifdef ION_LOGGER
		pApplication->GetServiceLocator().GetLogger()->Message(typeid(this).name(), Core::MsgType::Fatal, "Loading terrain failed " + mFileName);
#endif
		return;
	}
#endif
	heightsFile.read(reinterpret_cast<char*>(&mHeights[0]), static_cast<std::streamsize>(vertexCount * sizeof(unsigned short)));
	heightsFile.close();

	// Vertices
	Core::VertexPNU zero{};
	mVertices.assign(vertexCount, zero);
	float
		cellWidth{ mWidth / float(mColCount) },
		cellDepth{ mDepth / float(mRowCount) },
		cellXPos{ -mWidth / 2.f },
		cellZPos{ -mDepth / 2.f };
	for (std::size_t row{ 0 }; row < mRowCount; ++row)
	{
		cellZPos = mDepth / 2.f;
		cellZPos = -mDepth / 2.f;
		for (std::size_t col{ 0 }; col < mColCount; ++col)
		{
			std::size_t vertexId{ row * mColCount + col };
			mVertices[vertexId].mPosition.x = cellXPos;
			mVertices[vertexId].mPosition.y = float(mHeights[vertexId]) / 65536.f * mScale.y;
			mVertices[vertexId].mPosition.z = cellZPos;
			mVertices[vertexId].mUV.x = float(col) / float(mColCount - 1);
			mVertices[vertexId].mUV.y = float(row) / float(mRowCount - 1);
			cellZPos += cellDepth;
		}
		cellXPos += cellWidth;
	}
	// Indices
	{
		std::size_t
			quadsRowCount{ mRowCount - 1 },
			quadsColCount{ mColCount - 1 };
		mIndices.reserve(quadsRowCount * quadsColCount);
		DWORD
			a{ 0 },
			b{ 0 },
			c{ 0 },
			d{ 0 };
		for (std::size_t row{ 0 }; row < quadsRowCount; ++row)
			for (std::size_t col{ 0 }; col < quadsColCount; ++col)
			{
				a = DWORD(row * mColCount + col);
				b = DWORD(a + 1);
				c = DWORD(a + mColCount);
				d = DWORD(c + 1);
				mIndices.push_back(a);
				mIndices.push_back(d);
				mIndices.push_back(c);
				mIndices.push_back(a);
				mIndices.push_back(b);
				mIndices.push_back(d);
			}
	}
	// Normals
	{
		std::vector<DirectX::XMFLOAT3> faceNormals{};
		DirectX::XMVECTOR
			a{}, b{}, c{}, d{}, e{}, f{},
			v1{}, v2{},
			normal{};
		DirectX::XMFLOAT3 normalFloat3{};
		for (std::size_t i{ 0 }; i < mIndices.size(); i += 6)
		{
			a = DirectX::XMLoadFloat3(&mVertices[mIndices[i]].mPosition);
			b = DirectX::XMLoadFloat3(&mVertices[mIndices[i + 1]].mPosition);
			c = DirectX::XMLoadFloat3(&mVertices[mIndices[i + 2]].mPosition);
			d = DirectX::XMLoadFloat3(&mVertices[mIndices[i + 3]].mPosition);
			e = DirectX::XMLoadFloat3(&mVertices[mIndices[i + 4]].mPosition);
			f = DirectX::XMLoadFloat3(&mVertices[mIndices[i + 5]].mPosition);
			v1 = c - a;
			v2 = b - a;
			normal = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(v2, v1));
			DirectX::XMStoreFloat3(&normalFloat3, normal);
			faceNormals.push_back(normalFloat3);
			v1 = e - f;
			v2 = d - f;
			normal = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(v2, v1));
			DirectX::XMStoreFloat3(&normalFloat3, normal);
			faceNormals.push_back(normalFloat3);
		}
		int
			facesPerRowCount{ (int(mColCount) - 1) * 2 },
			centerIndex{ 0 },
			index[6];
		for (std::size_t row{ 0 }; row < mRowCount; ++row)
		{
			for (std::size_t col{ 0 }; col < mColCount; ++col)
			{
				centerIndex = facesPerRowCount * int(row) + int(col) * 2;
				index[0] = centerIndex - 1;
				index[1] = centerIndex;
				index[2] = centerIndex + 1;
				index[3] = centerIndex - facesPerRowCount - 2;
				index[4] = centerIndex - facesPerRowCount - 1;
				index[5] = centerIndex - facesPerRowCount;
				if (col == 0)
				{
					index[0] = -1;
					index[3] = -1;
					index[4] = -1;
				}
				if (col == mColCount - 1)
				{
					index[1] = -1;
					index[2] = -1;
					index[5] = -1;
				}
				DirectX::XMVECTOR sum{ 0.f, 0.f, 0.f };
				for (int i{ 0 }; i < 6; ++i)
					if ((index[i] >= 0) && (index[i] < int(faceNormals.size())))
						sum += DirectX::XMLoadFloat3(&faceNormals[index[i]]);
				DirectX::XMStoreFloat3(&mVertices[mColCount * row + col].mNormal, DirectX::XMVector3Normalize(sum));
			}
		}
	}

	{
               D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc{};
               cbvHeapDesc.NumDescriptors = 1;
               cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
               cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		mpObject->GetScene()->GetApplication()->ThrowIfFailed(pDevice->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&mpObjectCbvHeap)));
	}
	{
		const UINT indexBufferSize{ UINT(mIndices.size() * sizeof(DWORD)) };
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
		memcpy(mpIndexDataBegin, mIndices.data(), indexBufferSize);

		mIndexBufferView.BufferLocation = mIndexBuffer->GetGPUVirtualAddress();
		mIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
		mIndexBufferView.SizeInBytes = indexBufferSize;
	}
	{
		const UINT vertexBufferSize{ UINT(mVertices.size() * sizeof(Core::VertexPNU)) };
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
		memcpy(mpVertexDataBegin, mVertices.data(), vertexBufferSize);

		mVertexBufferView.BufferLocation = mVertexBuffer->GetGPUVirtualAddress();
		mVertexBufferView.StrideInBytes = sizeof(Core::VertexPNU);
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

       mCbvSrvDescriptorSize = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
       mCbvSrvOffset = pApplication->AllocateDescriptors(1 + 1 + UINT(mpTextureSrvHeaps.size()));
       {
               std::sort(mTextureTypeOrder.begin(), mTextureTypeOrder.end(), [](Core::TextureType a, Core::TextureType b) { return int(a) < int(b); });
               D3D12_CPU_DESCRIPTOR_HANDLE dest{ pApplication->GetCpuHandle(mCbvSrvOffset + 1) };
               pDevice->CopyDescriptorsSimple(1, dest, mpObjectCbvHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

       UINT offset{ 2 };
       for (auto textureType : mTextureTypeOrder)
       {
               auto& heap = mpTextureSrvHeaps[textureType];
               D3D12_CPU_DESCRIPTOR_HANDLE dst{ pApplication->GetCpuHandle(mCbvSrvOffset + offset) };
               pDevice->CopyDescriptorsSimple(1, dst, heap->GetCPUDescriptorHandleForHeapStart(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
               mTextureOffsets[textureType] = mCbvSrvOffset + offset;
               ++offset;
       }
       }

       // PhysX
	{
		physx::PxPhysics* physX{ pApplication->GetPxPhysics() };
		physx::PxCooking* pCooking{ pApplication->GetPxCooking() };

		physx::PxHeightFieldSample* pSamples{ new physx::PxHeightFieldSample[vertexCount]{} };
		for (std::size_t i{ 0 }; i < vertexCount; ++i)
		{
			pSamples[i].height = physx::PxI16(int(mHeights[i]) / 2);
			pSamples[i].setTessFlag();
			pSamples[i].materialIndex0 = 1;
			pSamples[i].materialIndex1 = 1;
		}
		physx::PxHeightFieldDesc hfDesc{};
		hfDesc.nbColumns = physx::PxU32(mColCount);
		hfDesc.nbRows = physx::PxU32(mRowCount);
		hfDesc.format = physx::PxHeightFieldFormat::eS16_TM;
		//hfDesc.convexEdgeThreshold = 0.f;
		//hfDesc.flags = physx::PxHeightFieldFlags{};
		hfDesc.samples.stride = sizeof(physx::PxHeightFieldSample);
		hfDesc.samples.data = pSamples;
		physx::PxHeightField* pHeightField{ pCooking->createHeightField(hfDesc, physX->getPhysicsInsertionCallback()) };
		physx::PxTransform pose{ physx::PxIdentity };
		pose.p = physx::PxVec3(-(float(mRowCount) / 2.f * mScale.x), 0, -(float(mColCount) / 2.f * mScale.z));
		physx::PxRigidStatic* pHfActor{ physX->createRigidStatic(pose) };
		physx::PxHeightFieldGeometry heightFieldGeom{ pHeightField, physx::PxMeshGeometryFlag::eDOUBLE_SIDED, 0.5f / mScale.y, mScale.x, mScale.z };
		physx::PxShape* pHfShape{ physx::PxRigidActorExt::createExclusiveShape(*pHfActor, heightFieldGeom, *physX->createMaterial(0.5f, 0.5f, 0.1f)) };
		(pHfShape);
		mpObject->GetScene()->GetPxScene()->addActor(*pHfActor);
		delete[] pSamples;
	}
	mIsInitialized = true;
}

void Core::TerrainVC::Update(float delta)
{
	(delta);
	if (!mIsActive)
		return;
}

bool Core::TerrainVC::Render(Core::Canvas* pCanvas, Core::Material3D* pMaterial, float distSq)
{
	if (!mIsInitialized)
	{
#ifdef ION_LOGGER
		mpObject->GetScene()->GetApplication()->GetServiceLocator().GetLogger()->Message(typeid(this).name(), Core::MsgType::Fatal, "MeshVC.Render() while mIsInitialized == false");
#endif
		return false;
	}
	if (!Core::ViewC::Render(pCanvas, pMaterial, distSq))
		return false;
	if (!mIsActive)
		return false;

        auto pGraphicsCommandList{ pCanvas->GetGraphicsCommandList() };
        auto pDevice{ mpObject->GetScene()->GetApplication()->GetDevice() };
        auto pApplication{ mpObject->GetScene()->GetApplication() };

        D3D12_CONSTANT_BUFFER_VIEW_DESC canvasCbv{};
        canvasCbv.BufferLocation = pCanvas->GetCanvasConstantBuffer()->GetGPUVirtualAddress();
        canvasCbv.SizeInBytes = sizeof(Core::CanvasConstantBuffer);
        pDevice->CreateConstantBufferView(&canvasCbv, pApplication->GetCpuHandle(mCbvSrvOffset));

        UINT dsTable{ 1 };
        SetDescTableObjectConstants(pCanvas, dsTable);
        SetDescTableTextures(pCanvas, dsTable);
	pGraphicsCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pGraphicsCommandList->IASetIndexBuffer(&mIndexBufferView);
	pGraphicsCommandList->IASetVertexBuffers(0, 1, &mVertexBufferView);
	pGraphicsCommandList->DrawIndexedInstanced(UINT(mIndices.size()), 1, 0, 0, 0);
	return true;
}

void Core::TerrainVC::SetDescTableObjectConstants(Core::Canvas* pCanvas, UINT& dsTable)
{
        auto pGraphicsCommandList{ pCanvas->GetGraphicsCommandList() };
        auto pApplication{ mpObject->GetScene()->GetApplication() };

        DirectX::XMMATRIX world{ DirectX::XMLoadFloat4x4(&mpObject->GetModelC<TransformMC>()->GetWorld()) };
        const DirectX::XMMATRIX viewProjection{ DirectX::XMLoadFloat4x4(&pCanvas->GetCamera()->GetModelC<CameraRMC>()->GetViewProjection()) };
        DirectX::XMMATRIX wvp{ world * viewProjection };

	DirectX::XMStoreFloat4x4(&mObjectConstantBufferData.mWorld, world);
	DirectX::XMStoreFloat4x4(&mObjectConstantBufferData.mWorldViewProj, wvp);

        memcpy(mpObjectCbvDataBegin, &mObjectConstantBufferData, sizeof(mObjectConstantBufferData));
        {
                D3D12_GPU_DESCRIPTOR_HANDLE handle{ pApplication->GetGpuHandle(mCbvSrvOffset + 1) };
                pGraphicsCommandList->SetGraphicsRootDescriptorTable(dsTable, handle);
        }
        ++dsTable;
}

void Core::TerrainVC::SetDescTableTextures(Core::Canvas* pCanvas, UINT& dsTable)
{
        auto pGraphicsCommandList{ pCanvas->GetGraphicsCommandList() };
        auto pApplication{ mpObject->GetScene()->GetApplication() };

        for (auto textureType : mTextureTypeOrder)
        {
                D3D12_GPU_DESCRIPTOR_HANDLE tex{ pApplication->GetGpuHandle(mTextureOffsets[textureType]) };
                pGraphicsCommandList->SetGraphicsRootDescriptorTable(dsTable, tex);
                ++dsTable;
        }
}
