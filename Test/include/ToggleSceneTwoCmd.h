#pragma once
#include "../Test/include/ControlZeroCommand.h"

namespace Ion
{
	namespace Core
	{
		class ToggleSceneTwoCmd final
			: public Core::ControlZeroCommand
		{
		public:
			explicit ToggleSceneTwoCmd(Core::ControlZeroRMC* pControlZeroRMC);
			virtual ~ToggleSceneTwoCmd() = default;
			ToggleSceneTwoCmd(const ToggleSceneTwoCmd& other) = default;
			ToggleSceneTwoCmd(ToggleSceneTwoCmd&& other) noexcept = default;
			ToggleSceneTwoCmd& operator=(const ToggleSceneTwoCmd& other) = default;
			ToggleSceneTwoCmd& operator=(ToggleSceneTwoCmd&& other) noexcept = default;

			virtual Core::ControlZeroCommand* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
