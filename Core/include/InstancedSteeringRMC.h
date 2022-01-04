#pragma once
#include "SteeringRMC.h"

// Instanced Steering Receiver Model Component

namespace Ion
{
	namespace Core
	{
		class Canvas;
		class InstancedTransformMC;

		class InstancedSteeringRMC final
			: public Core::SteeringRMC
		{
		public:
			explicit InstancedSteeringRMC(bool isActive, Core::Object* pObject);
			virtual ~InstancedSteeringRMC() = default;
			InstancedSteeringRMC(const InstancedSteeringRMC& other) = default;
			InstancedSteeringRMC(InstancedSteeringRMC&& other) noexcept = default;
			InstancedSteeringRMC& operator=(const InstancedSteeringRMC& other) = default;
			InstancedSteeringRMC& operator=(InstancedSteeringRMC&& other) noexcept = default;

			virtual void Initialize() override;
			virtual void Update(float delta) override;
			virtual void Switch() override;

			void SetInstancedTransformMC(Core::InstancedTransformMC* pInstancedTransform);
			void SetTarget(std::size_t index, TransformMC* pTarget);
		private:
			Core::InstancedTransformMC *mpInstancedTransform;
			std::vector<Core::TransformMC*> mpTargets;
			std::vector<Core::Velocity> mVelocities;
			std::vector<Core::WanderData> mWanderData;
		};
	}
}
