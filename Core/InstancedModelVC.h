#pragma once
#include "TextureType.h"
#include "ViewC.h"
#include "Triangle.h"
#include "VertexPNC.h"
#include "MeshVCConstantBuffer.h"
#include "Winding.h"
#include "InstanceBuffer.h"
#include "TransformMC.h"

// Model View Component

namespace Ion
{
	namespace Core
	{
		class Object;
		class Material3D;
		class Canvas3D;
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

			void AddTexture(TextureType textureType, const std::string& name);
			void AddInstance(const TransformMC& transform);
			void AddInstances(const std::vector<TransformMC>& transforms);
			void ReadInstances();
			std::vector<TransformMC>& GetInstances();

			virtual void Initialize() override;
			virtual void Update(float delta) override;
			virtual void Render(Canvas* pCanvas, Material3D* pMaterial) override;
			virtual void Render(Canvas* pCanvas, Material2D* pMaterial) override { (pCanvas); (pMaterial); };
		private:
			static const size_t mMaxInstances;
			Model* mpModel;
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
		};
	}
}
