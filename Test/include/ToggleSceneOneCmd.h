#pragma once
#include "../Test/include/ControlZeroCommand.h"

namespace Ion
{
	namespace Core
	{
		class ToggleSceneOneCmd final
			: public Core::ControlZeroCommand
		{
		public:
			explicit ToggleSceneOneCmd(Core::ControlZeroRMC* pControlZeroRMC);
			virtual ~ToggleSceneOneCmd() = default;
			ToggleSceneOneCmd(const ToggleSceneOneCmd& other) = default;
			ToggleSceneOneCmd(ToggleSceneOneCmd&& other) noexcept = default;
			ToggleSceneOneCmd& operator=(const ToggleSceneOneCmd& other) = default;
			ToggleSceneOneCmd& operator=(ToggleSceneOneCmd&& other) noexcept = default;

			virtual Core::ControlZeroCommand* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
