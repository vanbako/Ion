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
			explicit MeshVC(const std::string& materialName, bool isActive, Core::Object* pObject);
			virtual ~MeshVC() = default;
			MeshVC(const MeshVC& other) = default;
			MeshVC(MeshVC&& other) noexcept = default;
			MeshVC& operator=(const MeshVC& other) = default;
			MeshVC& operator=(MeshVC&& other) noexcept = default;

			void AddTriangle(const Core::VertexPNC& a, const Core::VertexPNC& b, const Core::VertexPNC& c);
			void AddQuadrilateral(const Core::Quadrilateral<Core::VertexPNC>& quadrilateral);

			virtual void Initialize() override;
			virtual void Update(float delta) override;
			virtual bool Render(Core::Canvas* pCanvas, Core::Material3D* pMaterial) override;
			virtual bool Render(Core::Canvas* pCanvas, Core::Material2D* pMaterial) override { (pCanvas); (pMaterial); return false; };
		protected:
			static const size_t mMaxVertices{ 900 };

			bool mMeshChanged;
			Core::VertexPNC mVertices[mMaxVertices];
			size_t mVertexCount;

			Microsoft::WRL::ComPtr<ID3D12Resource> mVertexBuffer;
			D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;
			UINT8* mpVertexDataBegin;

			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mpObjectCbvHeap;
			Microsoft::WRL::ComPtr<ID3D12Resource> mpObjectConstantBuffer;
			Core::MeshVCConstantBuffer mObjectConstantBufferData;
			UINT8* mpObjectCbvDataBegin;
		};
	}
}
