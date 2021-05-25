#pragma once
#include "../Core/Component.h"

// View Component

namespace Ion
{
	namespace Core
	{
		class Canvas;
		class Material;

		class ViewC
			: public Component
		{
		public:
			explicit ViewC(bool isActive, Core::Object* pObject);
			virtual ~ViewC() = default;
			ViewC(const ViewC& other) = default;
			ViewC(ViewC&& other) noexcept = default;
			ViewC& operator=(const ViewC& other) = default;
			ViewC& operator=(ViewC&& other) noexcept = default;

			virtual void Render(Canvas* pCanvas, Material* pMaterial) = 0;
		};
	}
}
