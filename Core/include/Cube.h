#pragma once
#include "Vector.h"
#include "CubeCanvasCamera.h"

namespace Ion
{
	namespace Core
	{
		class Canvas;
		class Scene;

		class Cube
		{
		public:
			explicit Cube(const Core::Vector<long long>& location);
			explicit Cube();
			virtual ~Cube() = default;
			Cube(const Cube& other) = delete;
			Cube(Cube&& other) noexcept = delete;
			Cube& operator=(const Cube& other) = delete;
			Cube& operator=(Cube&& other) noexcept = delete;

			void SetScene(Core::Scene* pScene);
			Core::Scene* GetScene();
			Core::Vector<long long>& GetLocation();
			void SetLocation(const Core::Vector<long long>& loc);
		protected:
			Core::Vector<long long> mLocation;
			std::vector<CubeCanvasCamera> mCubeCanvasCameras;
			Core::Scene* mpScene;

			void UpdateCanvasCamera(Core::Canvas* pCanvas);
		};
	}
}
