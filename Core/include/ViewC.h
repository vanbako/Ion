#pragma once
#include "Component.h"

// View Component

namespace Ion
{
	namespace Core
	{
		class Canvas;
		class Material3D;
		class Material2D;

		class ViewC
			: public Component
		{
		public:
			explicit ViewC(bool isActive, Object* pObject, const std::string& material3D, const std::string& material2D);
			virtual ~ViewC() = default;
			ViewC(const ViewC& other) = default;
			ViewC(ViewC&& other) noexcept = default;
			ViewC& operator=(const ViewC& other) = default;
			ViewC& operator=(ViewC&& other) noexcept = default;

			void AddCanvas(Canvas* pCanvas);

			virtual void Render(Canvas* pCanvas, Material3D* pMaterial) = 0;
			virtual void Render(Canvas* pCanvas, Material2D* pMaterial) = 0;
		protected:
			Material3D* mpMaterial3D;
			Material2D* mpMaterial2D;
			std::set<Canvas*> mpCanvases;
		};
	}
}
