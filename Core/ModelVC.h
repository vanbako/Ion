#pragma once
#include "../Core/ViewC.h"
#include "../Core/Triangle.h"
#include "../Core/VertexPNC.h"
#include "../Core/MeshVCConstantBuffer.h"

// Model View Component

namespace Ion
{
	namespace Core
	{
		class Object;
		class Material;
		class Canvas;
		class Model;

		class ModelVC
			: public ViewC
		{
		public:
			explicit ModelVC(const std::string& modelName, const std::string& materialName, bool isActive, Object* pObject);
			virtual ~ModelVC();
			ModelVC(const ModelVC& other) = default;
			ModelVC(ModelVC&& other) noexcept = default;
			ModelVC& operator=(const ModelVC& other) = default;
			ModelVC& operator=(ModelVC&& other) noexcept = default;

			void AddCanvas(Canvas* pCanvas);

			virtual void Initialize() override;
			virtual void Update(float delta) override;
			virtual void Render(Canvas* pCanvas, Material* pMaterial) override;
		private:
			Model* mpModel;
			Material* mpMaterial;
			char* mpVertices;
			size_t
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
			MeshVCConstantBuffer mObjectConstantBufferData;
			UINT8* mpObjectCbvDataBegin;

			std::set<Canvas*> mpCanvases;
		};
	}
}
