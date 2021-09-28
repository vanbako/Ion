#pragma once
#include "SceneThread.h"

namespace Ion
{
	namespace Core
	{
		class Command;
		class InputCC;

		class PhysicsST final
			: public Core::SceneThread
		{
		public:
			explicit PhysicsST(Core::Scene* pScene, std::chrono::milliseconds updateTime);
			virtual ~PhysicsST() = default;
			PhysicsST(const PhysicsST& other) = delete;
			PhysicsST(PhysicsST&& other) noexcept = delete;
			PhysicsST& operator=(const PhysicsST& other) = delete;
			PhysicsST& operator=(PhysicsST&& other) noexcept = delete;
		protected:
			virtual void Inner(float delta) override;
		};
	}
}
