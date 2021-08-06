#pragma once
#include "SteeringCommand.h"

namespace Ion
{
	namespace Core
	{
		class WanderCmd final
			: public Core::SteeringCommand
		{
		public:
			explicit WanderCmd(Core::SteeringRMC* pSteeringRMC);
			virtual ~WanderCmd() = default;
			WanderCmd(const WanderCmd& other) = default;
			WanderCmd(WanderCmd&& other) noexcept = default;
			WanderCmd& operator=(const WanderCmd& other) = default;
			WanderCmd& operator=(WanderCmd&& other) noexcept = default;

			virtual Core::WanderCmd* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
