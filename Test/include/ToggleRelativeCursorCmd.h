#pragma once
#include "../Test/include/ControlZeroCommand.h"

namespace Ion
{
	namespace Core
	{
		class ToggleRelativeCursorCmd final
			: public Core::ControlZeroCommand
		{
		public:
			explicit ToggleRelativeCursorCmd(Core::ControlZeroRMC* pControlZeroRMC);
			virtual ~ToggleRelativeCursorCmd() = default;
			ToggleRelativeCursorCmd(const ToggleRelativeCursorCmd& other) = default;
			ToggleRelativeCursorCmd(ToggleRelativeCursorCmd&& other) noexcept = default;
			ToggleRelativeCursorCmd& operator=(const ToggleRelativeCursorCmd& other) = default;
			ToggleRelativeCursorCmd& operator=(ToggleRelativeCursorCmd&& other) noexcept = default;

			virtual Core::ControlZeroCommand* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
