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
		class ViewCCube;

		class ViewC
			: public Core::Component
		{
		public:
			explicit ViewC(bool isActive, Core::Object* pObject, const std::string& material3D, const std::string& material2D);
			virtual ~ViewC();
			ViewC(const ViewC& other) = default;
			ViewC(ViewC&& other) noexcept = default;
			ViewC& operator=(const ViewC& other) = default;
			ViewC& operator=(ViewC&& other) noexcept = default;

			virtual void Update(float) override;

			void SetCube(Core::ViewCCube* pCube);
			Core::ViewCCube* GetCube();
			void AddCanvas(Core::Canvas* pCanvas);

			virtual bool Render(Core::Canvas* pCanvas, Core::Material3D* pMaterial, float dist);
			virtual bool Render(Core::Canvas* pCanvas, Core::Material2D* pMaterial);
		protected:
			std::string
				mMaterial3D,
				mMaterial2D;
			Core::Material3D* mpMaterial3D;
			Core::Material2D* mpMaterial2D;
			std::set<Core::Canvas*> mpCanvases;
			Core::ViewCCube* mpCube;
		};
	}
}
