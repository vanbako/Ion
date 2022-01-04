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
		class Model;
		class Texture;

		class ModelVC
			: public ViewC
		{
		public:
			explicit ModelVC(const std::string& modelName, const std::string& modelExtension, const std::string& materialName, bool isActive, Core::Winding winding, Core::CoordSystem coordSystem, Core::Object* pObject);
			virtual ~ModelVC();
			ModelVC(const ModelVC& other) = default;
			ModelVC(ModelVC&& other) noexcept = default;
			ModelVC& operator=(const ModelVC& other) = default;
			ModelVC& operator=(ModelVC&& other) noexcept = default;

			void AddTexture(Core::TextureType textureType, const std::string& name);
			void SetShininess(float shininess);
			virtual void Initialize() override;
			virtual void Update(float delta) override;
			virtual bool Render(Core::Canvas* pCanvas, Core::Material3D* pMaterial) override;
			virtual bool Render(Core::Canvas* pCanvas, Core::Material2D* pMaterial) override { (pCanvas); (pMaterial); return false; };
		protected:
			std::string mName;
			Core::Model* mpModel;
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
			std::map<Core::TextureType, Core::Texture*> mpTextures;
			std::map<Core::TextureType, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>> mpTextureSrvHeaps;

			void SetDescTableObjectConstants(Core::Canvas* pCanvas, UINT& dsTable);
			void SetDescTableTextures(Core::Canvas* pCanvas, UINT& dsTable);
		};
	}
}
