#pragma once
#include "Command.h"

namespace Ion
{
	namespace Core
	{
		class SteeringRMC;

		class SteeringCommand
			: public Core::Command
		{
		public:
			virtual ~SteeringCommand() = default;
			SteeringCommand(const SteeringCommand& other) = default;
			SteeringCommand(SteeringCommand&& other) noexcept = default;
			SteeringCommand& operator=(const SteeringCommand& other) = default;
			SteeringCommand& operator=(SteeringCommand&& other) noexcept = default;

			virtual Core::SteeringCommand* Duplicate() = 0;
			virtual void Execute() = 0;
		protected:
			explicit SteeringCommand(Core::SteeringRMC* pMoveRMC);

			Core::SteeringRMC* mpSteeringRMC;
		};
	}
}
