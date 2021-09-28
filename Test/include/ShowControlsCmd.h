#pragma once
#include "ControlOneCommand.h"

namespace Ion
{
	namespace Core
	{
		class ShowControlsCmd final
			: public Core::ControlOneCommand
		{
		public:
			explicit ShowControlsCmd(Core::ControlOneRMC* pControlOneRMC);
			virtual ~ShowControlsCmd() = default;
			ShowControlsCmd(const ShowControlsCmd& other) = default;
			ShowControlsCmd(ShowControlsCmd&& other) noexcept = default;
			ShowControlsCmd& operator=(const ShowControlsCmd& other) = default;
			ShowControlsCmd& operator=(ShowControlsCmd&& other) noexcept = default;

			virtual Core::ControlOneCommand* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
