#pragma once
#include "TextureType.h"
#include "ViewC.h"
#include "Triangle.h"
#include "VertexPNC.h"
#include "MeshVCConstantBuffer.h"
#include "Winding.h"
#include "CoordSystem.h"

// Model View Component

namespace Ion
{
	namespace Core
	{
		class Object;
		class Material3D;
		class Canvas;
		class MeshModel;
		class Texture;

		class MeshModelVC
			: public ViewC
		{
		public:
			explicit MeshModelVC(const std::string& modelName, const std::string& modelExtension, const std::string& materialName, bool isActive, Core::Winding winding, Core::CoordSystem coordSystem, Core::Object* pObject);
			virtual ~MeshModelVC();
			MeshModelVC(const MeshModelVC& other) = default;
			MeshModelVC(MeshModelVC&& other) noexcept = default;
			MeshModelVC& operator=(const MeshModelVC& other) = default;
			MeshModelVC& operator=(MeshModelVC&& other) noexcept = default;

			void AddTexture(Core::TextureType textureType, const std::string& name);
			void SetShininess(float shininess);
			virtual void Initialize() override;
			virtual void Update(float delta) override;
			virtual bool Render(Core::Canvas* pCanvas, Core::Material3D* pMaterial, float distSq) override;
			virtual bool Render(Core::Canvas* pCanvas, Core::Material2D* pMaterial) override { (pCanvas); (pMaterial); return false; };
		protected:
			std::string mName;
			Core::MeshModel* mpMeshModel;
			char* mpVertices;
			std::size_t
				mIndexCount,
				mVertexCount;

			Microsoft::WRL::ComPtr<ID3D12Resource> mIndexBuffer;
			D3D12_INDEX_BUFFER_VIEW mIndexBufferView;
			UINT8* mpIndexDataBegin;

			Microsoft::WRL::ComPtr<ID3D12Resource> mVertexBuffer;
			D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;
			UINT8* mpVertexDataBegin;

			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mpObjectCbvHeap;
			Microsoft::WRL::ComPtr<ID3D12Resource> mpObjectConstantBuffer;
			Core::MeshVCConstantBuffer mObjectConstantBufferData;
			UINT8* mpObjectCbvDataBegin;

                       std::vector<std::string> mTextureNames;
                       std::vector<Core::TextureType> mTextureTypeOrder;
                       std::unordered_map<Core::TextureType, Core::Texture*> mpTextures;
                       std::unordered_map<Core::TextureType, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>> mpTextureSrvHeaps;

                       UINT mCbvSrvOffset;
                       UINT mCbvSrvDescriptorSize;
                       std::unordered_map<Core::TextureType, UINT> mTextureOffsets;

			void SetDescTableObjectConstants(Core::Canvas* pCanvas, UINT& dsTable);
			void SetDescTableTextures(Core::Canvas* pCanvas, UINT& dsTable);
		};
	}
}
