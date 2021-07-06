#pragma once
#include "../Core/ViewC.h"
#include "../Core/Triangle.h"

// Triangle View Component

namespace Ion
{
	namespace Core
	{
		class Object;
		class Material2D;
		class Canvas2D;

		class TriangleVC
			: public ViewC
		{
		public:
			explicit TriangleVC(bool isActive, Object* pObject);
			virtual ~TriangleVC() = default;
			TriangleVC(const TriangleVC& other) = default;
			TriangleVC(TriangleVC&& other) noexcept = default;
			TriangleVC& operator=(const TriangleVC& other) = default;
			TriangleVC& operator=(TriangleVC&& other) noexcept = default;

			virtual void Initialize() override;
			virtual void Update(float delta) override;
			virtual void Render(Canvas* pCanvas, Material3D* pMaterial) override { (pCanvas); (pMaterial); };
			virtual void Render(Canvas* pCanvas, Material2D* pMaterial) override;
		private:
			Microsoft::WRL::ComPtr<ID2D1PathGeometry> mpPathGeometry;
		};
	}
}
