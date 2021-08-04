#pragma once
#include "SteeringRMC.h"

// Steering Receiver Model Component

namespace Ion
{
	namespace Core
	{
		class Canvas;
		class TransformMC;

		class ObjectSteeringRMC final
			: public Core::SteeringRMC
		{
		public:
			explicit ObjectSteeringRMC(bool isActive, Core::Object* pObject);
			virtual ~ObjectSteeringRMC() = default;
			ObjectSteeringRMC(const ObjectSteeringRMC& other) = default;
			ObjectSteeringRMC(ObjectSteeringRMC&& other) noexcept = default;
			ObjectSteeringRMC& operator=(const ObjectSteeringRMC& other) = default;
			ObjectSteeringRMC& operator=(ObjectSteeringRMC&& other) noexcept = default;

			virtual void Initialize() override;
			virtual void Update(float delta) override;
			virtual void Switch() override;

			void SetTransformMC(TransformMC* pTransform);
		private:
			Core::TransformMC *mpTransform;
			Core::Velocity mVelocity;
			WanderData mWander;
		};
	}
}
