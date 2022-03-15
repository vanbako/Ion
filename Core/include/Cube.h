#pragma once
#include "Vector.h"
#include "CubeViewC.h"

namespace Ion
{
	namespace Core
	{
		class ViewC;
		class Canvas;
		class Material3D;

		class Cube final
		{
		public:
			explicit Cube(Core::Material3D* pMaterial, Core::Vector<long long> location);
			explicit Cube(Core::Material3D* pMaterial);
			~Cube() = default;
			Cube(const Cube& other) = delete;
			Cube(Cube&& other) noexcept = delete;
			Cube& operator=(const Cube& other) = delete;
			Cube& operator=(Cube&& other) noexcept = delete;

			void SetHasMoved(Core::ViewC* pViewC, bool hasMoved);
			void AddViewC(bool hasMoved, Core::ViewC* pViewC);
			void AddViewCCheckExistence(bool hasMoved, Core::ViewC* pViewC);
			Core::Vector<long long>& GetLocation();
			void SetLocation(const Core::Vector<long long>& loc);
			void Render(Canvas* pCanvas, Material3D* pMaterial);
		private:
			Core::Material3D* mpMaterial;
			Core::Vector<long long> mLocation;
			std::vector<CubeViewC> mCubeViewCs;
		};
	}
}
