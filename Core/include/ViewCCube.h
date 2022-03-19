#pragma once
#include "Cube.h"

namespace Ion
{
	namespace Core
	{
		class ViewC;
		class Canvas;
		class Material3D;

		struct CubeViewC
		{
			bool mHasMoved;
			ViewC* mpViewC;
		};
		struct CanvasCamera
		{
			Canvas* mpCanvas;
			float mDot;
			float mDist;
		};

		class ViewCCube
			: public Cube
		{
		public:
			explicit ViewCCube(const Core::Vector<long long>& location);
			explicit ViewCCube();
			~ViewCCube() = default;
			ViewCCube(const ViewCCube& other) = delete;
			ViewCCube(ViewCCube&& other) noexcept = delete;
			ViewCCube& operator=(const ViewCCube& other) = delete;
			ViewCCube& operator=(ViewCCube&& other) noexcept = delete;

			void SetMaterial3D(Core::Material3D* pMaterial3D);
			Core::Material3D* GetMaterial3D();
			void SetHasMoved(Core::ViewC* pViewC, bool hasMoved);
			void AddViewC(bool hasMoved, Core::ViewC* pViewC);
			void AddViewCCheckExistence(bool hasMoved, Core::ViewC* pViewC);
			void Render(Canvas* pCanvas, Material3D* pMaterial);
			void ViewCUpdate(Canvas* pCanvas, float delta);
		private:
			static const float mBackRenderDist;
			Core::Material3D* mpMaterial3D;
			std::vector<CubeViewC> mCubeViewCs;
			std::vector<CanvasCamera> mCanvasCameras;

			Core::CanvasCamera* GetCanvasCamera(Core::Canvas* pCanvas);
		};
	}
}
