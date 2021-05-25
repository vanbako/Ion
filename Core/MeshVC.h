#pragma once
#include "../Core/ViewC.h"
#include "../Core/Triangle.h"
#include "../Core/VertexPNC.h"
#include "../Core/Quadrilateral.h"
#include "../Core/Material.h"
#include "../Core/MeshVCConstantBuffer.h"

// Mesh View Component

namespace Ion
{
	namespace Core
	{
		class Object;
		class Material;
		class Canvas;

		class MeshVC
			: public ViewC
		{
		public:
			explicit MeshVC(bool isActive, Object* pObject);
			virtual ~MeshVC();
			MeshVC(const MeshVC& other) = default;
			MeshVC(MeshVC&& other) noexcept = default;
			MeshVC& operator=(const MeshVC& other) = default;
			MeshVC& operator=(MeshVC&& other) noexcept = default;

			void AddCanvas(Canvas* pCanvas);
			void AddTriangle(const VertexPNC& a, const VertexPNC& b, const VertexPNC& c);
			void AddQuadrilateral(const Quadrilateral<VertexPNC>& quadrilateral);

			virtual void Initialize() override;
			virtual void Update(float delta) override;
			virtual void Render(Canvas* pCanvas, Material* pMaterial) override;
		private:
			static const size_t mMaxVertices{ 900 };

			bool mMeshChanged;
			VertexPNC mVertices[mMaxVertices];
			size_t mVertexCount;
			Material* mpMaterial;

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
