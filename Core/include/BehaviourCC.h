#pragma once
#include "ControllerC.h"
#include "Behaviour.h"

// Behaviour Controller Component

namespace Ion
{
	namespace Core
	{
		class WanderCmd;
		class SeekCmd;
		class SteeringRMC;

		class BehaviourCC
			: public Core::ControllerC
		{
		public:
			explicit BehaviourCC(bool isActive, Core::Object* pObject);
			virtual ~BehaviourCC();
			BehaviourCC(const BehaviourCC& other) = default;
			BehaviourCC(BehaviourCC&& other) noexcept = default;
			BehaviourCC& operator=(const BehaviourCC& other) = default;
			BehaviourCC& operator=(BehaviourCC&& other) noexcept = default;

			virtual void Update(float delta) override;

			void SetSteeringRMC(SteeringRMC* pSteeringRMC);
			void SetBehaviour(Behaviour behaviour);
		private:
			Behaviour mBehaviour;
			SteeringRMC* mpSteeringRMC;
			WanderCmd* mpWanderCmd;
			SeekCmd* mpSeekCmd;
		};
	}
}
