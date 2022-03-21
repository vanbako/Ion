#pragma once
#include "Cube.h"

namespace Ion
{
	namespace Core
	{
		class ControllerC;

		struct CubeControllerC
		{
			bool mHasMoved;
			ControllerC* mpControllerC;
		};

		class ControllerCCube
			: public Cube
		{
		public:
			explicit ControllerCCube(const Core::Vector<long long>& location);
			explicit ControllerCCube();
			~ControllerCCube() = default;
			ControllerCCube(const ControllerCCube& other) = delete;
			ControllerCCube(ControllerCCube&& other) noexcept = delete;
			ControllerCCube& operator=(const ControllerCCube& other) = delete;
			ControllerCCube& operator=(ControllerCCube&& other) noexcept = delete;

			void SetHasMoved(Core::ControllerC* pControllerC, bool hasMoved);
			void AddControllerC(bool hasMoved, Core::ControllerC* pControllerC);
			void AddControllerCCheckExistence(bool hasMoved, Core::ControllerC* pControllerC);
			void ControllerCUpdate(float delta);
		private:
			static const float mControllerUpdateDist;
			std::vector<CubeControllerC> mCubeControllerCs;
		};
	}
}
