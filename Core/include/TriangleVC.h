#pragma once
#include "ViewC.h"
#include "Triangle.h"

// Triangle View Component

namespace Ion
{
	namespace Core
	{
		class Object;
		class Material2D;
		class Canvas2D;

		class TriangleVC
			: public Core::ViewC
		{
		public:
			explicit TriangleVC(bool isActive, Core::Object* pObject);
			virtual ~TriangleVC() = default;
			TriangleVC(const TriangleVC& other) = default;
			TriangleVC(TriangleVC&& other) noexcept = default;
			TriangleVC& operator=(const TriangleVC& other) = default;
			TriangleVC& operator=(TriangleVC&& other) noexcept = default;

			virtual void Initialize() override;
			virtual void Update(float delta) override;
			virtual bool Render(Core::Canvas* pCanvas, Core::Material3D* pMaterial) override { (pCanvas); (pMaterial); return false; };
			virtual bool Render(Core::Canvas* pCanvas, Core::Material2D* pMaterial) override;
		private:
			Microsoft::WRL::ComPtr<ID2D1PathGeometry> mpPathGeometry;
		};
	}
}
