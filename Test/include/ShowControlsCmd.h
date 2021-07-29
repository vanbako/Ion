#pragma once
#include "ControlCommand.h"

namespace Ion
{
	namespace Core
	{
		class ShowControlsCmd final
			: public Core::ControlCommand
		{
		public:
			explicit ShowControlsCmd(Core::ControlRMC* pControlRMC);
			virtual ~ShowControlsCmd() = default;
			ShowControlsCmd(const ShowControlsCmd& other) = default;
			ShowControlsCmd(ShowControlsCmd&& other) noexcept = default;
			ShowControlsCmd& operator=(const ShowControlsCmd& other) = default;
			ShowControlsCmd& operator=(ShowControlsCmd&& other) noexcept = default;

			virtual Core::ControlCommand* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
