#pragma once
#include "ViewC.h"
#include "Triangle.h"
#include "VertexPNC.h"
#include "Quadrilateral.h"
#include "MeshVCConstantBuffer.h"

// Mesh View Component

namespace Ion
{
	namespace Core
	{
		class Object;
		class Material3D;
		class Canvas;

		class MeshVC
			: public ViewC
		{
		public:
			explicit MeshVC(bool isActive, Object* pObject);
			virtual ~MeshVC() = default;
			MeshVC(const MeshVC& other) = default;
			MeshVC(MeshVC&& other) noexcept = default;
			MeshVC& operator=(const MeshVC& other) = default;
			MeshVC& operator=(MeshVC&& other) noexcept = default;

			void AddTriangle(const VertexPNC& a, const VertexPNC& b, const VertexPNC& c);
			void AddQuadrilateral(const Quadrilateral<VertexPNC>& quadrilateral);

			virtual void Initialize() override;
			virtual void Update(float delta) override;
			virtual void Render(Canvas* pCanvas, Material3D* pMaterial) override;
			virtual void Render(Canvas* pCanvas, Material2D* pMaterial) override { (pCanvas); (pMaterial); };
		private:
			static const size_t mMaxVertices{ 900 };

			bool mMeshChanged;
			VertexPNC mVertices[mMaxVertices];
			size_t mVertexCount;

			Microsoft::WRL::ComPtr<ID3D12Resource> mVertexBuffer;
			D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;
			UINT8* mpVertexDataBegin;

			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mpObjectCbvHeap;
			Microsoft::WRL::ComPtr<ID3D12Resource> mpObjectConstantBuffer;
			MeshVCConstantBuffer mObjectConstantBufferData;
			UINT8* mpObjectCbvDataBegin;
		};
	}
}
