#pragma once
#include "ReceiverMC.h"
#include "Velocity.h"

// Steering Receiver Model Component

namespace Ion
{
	namespace Core
	{
		class Canvas;
		class TransformMC;

		class SteeringRMC final
			: public Core::ReceiverMC
		{
		public:
			explicit SteeringRMC(bool isActive, Core::Object* pObject);
			virtual ~SteeringRMC();
			SteeringRMC(const SteeringRMC& other) = default;
			SteeringRMC(SteeringRMC&& other) noexcept = default;
			SteeringRMC& operator=(const SteeringRMC& other) = default;
			SteeringRMC& operator=(SteeringRMC&& other) noexcept = default;

			virtual void Initialize() override;
			virtual void Update(float delta) override;
			virtual void Switch() override;
			virtual const std::vector<std::pair<std::string, Core::Command*>>& GetCommands() const override;
			virtual const std::string& GetName() const override;

			void SetTransformMC(TransformMC* pTransform);

			virtual void Wander(float value);
		private:
			static const std::string mName;
			std::vector<std::pair<std::string, Core::Command*>> mCommands;
			Core::TransformMC *mpTransform;
			Core::Velocity mVelocity;
			float
				mOffset,
				mRadius,
				mAngleChange,
				mWanderAngle;
			// My best guess is that boolean operations are thread-safe
			// Implement std::atomic if needed
			std::vector<float> mWanderDeltas;

			float GetOrientationFromVelocity(const Core::Velocity& velocity);
			void Steering(Core::Velocity velocity, float delta);
			Core::Velocity CalculateWander(float delta);
		};
	}
}
