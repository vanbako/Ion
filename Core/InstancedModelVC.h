#pragma once
#include "../Core/TextureType.h"
#include "../Core/ViewC.h"
#include "../Core/Triangle.h"
#include "../Core/VertexPNC.h"
#include "../Core/MeshVCConstantBuffer.h"
#include "../Core/Winding.h"
#include "../Core/InstanceBuffer.h"
#include "../Core/TransformMC.h"

// Model View Component

namespace Ion
{
	namespace Core
	{
		class Object;
		class Material;
		class Canvas;
		class Model;
		class Texture;

		class InstancedModelVC
			: public ViewC
		{
		public:
			explicit InstancedModelVC(const std::string& modelName, const std::string& materialName, bool isActive, Winding winding, Object* pObject);
			virtual ~InstancedModelVC();
			InstancedModelVC(const InstancedModelVC& other) = default;
			InstancedModelVC(InstancedModelVC&& other) noexcept = default;
			InstancedModelVC& operator=(const InstancedModelVC& other) = default;
			InstancedModelVC& operator=(InstancedModelVC&& other) noexcept = default;

			void AddCanvas(Canvas* pCanvas);
			void AddTexture(TextureType textureType, const std::string& name);

			virtual void Initialize() override;
			virtual void Update(float delta) override;
			virtual void Render(Canvas* pCanvas, Material* pMaterial) override;
		private:
			static const size_t mMaxInstances;
			Model* mpModel;
			Material* mpMaterial;
			std::map<TextureType,Texture*> mpTextures;
			char* mpVertices;
			size_t
				mIndexCount,
				mVertexCount;
			std::vector<TransformMC> mTransforms;

			Microsoft::WRL::ComPtr<ID3D12Resource> mIndexBuffer;
			D3D12_INDEX_BUFFER_VIEW mIndexBufferView;
			UINT8* mpIndexDataBegin;

			Microsoft::WRL::ComPtr<ID3D12Resource> mVertexBuffer;
			D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;
			UINT8* mpVertexDataBegin;

			Microsoft::WRL::ComPtr<ID3D12Resource> mpInstanceBuffer;
			std::vector<InstanceBuffer> mInstanceBufferData;
			UINT8* mpInstanceDataBegin;

			std::map<TextureType, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>> mpTextureSrvHeaps;

			std::set<Canvas*> mpCanvases;
		};
	}
}
