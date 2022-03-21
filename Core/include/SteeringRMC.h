#pragma once
#include "ReceiverMC.h"
#include "Velocity.h"
#include "WanderData.h"

// Steering Receiver Model Component

namespace Ion
{
	namespace Core
	{
		class TransformMC;

		class SteeringRMC
			: public Core::ReceiverMC
		{
		public:
			explicit SteeringRMC(bool isActive, Core::Object* pObject);
			virtual ~SteeringRMC();
			SteeringRMC(const SteeringRMC& other) = default;
			SteeringRMC(SteeringRMC&& other) noexcept = default;
			SteeringRMC& operator=(const SteeringRMC& other) = default;
			SteeringRMC& operator=(SteeringRMC&& other) noexcept = default;

			virtual void Update(float delta) = 0;
			virtual void Switch() override = 0;
			virtual const std::vector<std::pair<std::string, Core::Command*>>& GetCommands() const override;
			virtual const std::string& GetName() const override;

			virtual void Wander(float value);
			virtual void Seek(float value);
		protected:
			static const std::string mName;
			static const float mMaxSpeed;
			std::vector<std::pair<std::string, Core::Command*>> mCommands;
			// TODO: Make these vectors thread-safe
			std::vector<float> mWanderDeltas;
			std::vector<float> mSeekDeltas;

			virtual float GetOrientationFromVelocity(const Core::Velocity& velocity);
			virtual void Steering(TransformMC* pTransform, Core::Velocity& velocity, Core::Velocity steeringVelocity, float delta);
			virtual Core::Velocity CalculateWander(Core::WanderData& wanderData, Core::Velocity& velocity);
			virtual Core::Velocity CalculateSeek(TransformMC* pTransform, TransformMC* pTarget);
		};
	}
}
