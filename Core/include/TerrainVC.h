#pragma once
#include "ViewC.h"
#include "TextureType.h"
#include "Triangle.h"
#include "VertexPNT.h"
#include "Quadrilateral.h"
#include "MeshVCConstantBuffer.h"

// Terrain Mesh View Component

namespace Ion
{
	namespace Core
	{
		class Texture;

		class TerrainVC final
			: public ViewC
		{
		public:
			explicit TerrainVC(const std::string& filename, float width, float depth, std::size_t rowCount, std::size_t colCount, bool isActive, Core::Object* pObject);
			virtual ~TerrainVC() = default;
			TerrainVC(const TerrainVC& other) = default;
			TerrainVC(TerrainVC&& other) noexcept = default;
			TerrainVC& operator=(const TerrainVC& other) = default;
			TerrainVC& operator=(TerrainVC&& other) noexcept = default;

			void AddTexture(Core::TextureType textureType, const std::string& name);

			virtual void Initialize() override;
			virtual void Update(float delta) override;
			virtual void Render(Core::Canvas* pCanvas, Core::Material3D* pMaterial) override;
			virtual void Render(Core::Canvas* pCanvas, Core::Material2D* pMaterial) override { (pCanvas); (pMaterial); };
		private:
			const std::string mFileName;
			const float
				mWidth,
				mDepth;
			const std::size_t
				mRowCount,
				mColCount;
			
			std::vector<unsigned short> mHeights;

			std::vector<DWORD> mIndices;
			Microsoft::WRL::ComPtr<ID3D12Resource> mIndexBuffer;
			D3D12_INDEX_BUFFER_VIEW mIndexBufferView;
			UINT8* mpIndexDataBegin;

			std::vector<Core::VertexPNT> mVertices;
			Microsoft::WRL::ComPtr<ID3D12Resource> mVertexBuffer;
			D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;
			UINT8* mpVertexDataBegin;

			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mpObjectCbvHeap;
			Microsoft::WRL::ComPtr<ID3D12Resource> mpObjectConstantBuffer;
			Core::MeshVCConstantBuffer mObjectConstantBufferData;
			UINT8* mpObjectCbvDataBegin;

			std::map<Core::TextureType, Core::Texture*> mpTextures;
			std::map<Core::TextureType, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>> mpTextureSrvHeaps;

			void SetDescTableObjectConstants(Core::Canvas* pCanvas, UINT& dsTable);
			void SetDescTableTextures(Core::Canvas* pCanvas, UINT& dsTable);
		};
	}
}