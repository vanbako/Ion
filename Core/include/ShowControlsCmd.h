#pragma once
#include "Command.h"

namespace Ion
{
	namespace Core
	{
		class ReceiverMC;

		class ShowControlsCmd final
			: public Core::Command
		{
		public:
			explicit ShowControlsCmd(Core::ReceiverMC* pReceiver);
			virtual ~ShowControlsCmd() = default;
			ShowControlsCmd(const ShowControlsCmd& other) = default;
			ShowControlsCmd(ShowControlsCmd&& other) noexcept = default;
			ShowControlsCmd& operator=(const ShowControlsCmd& other) = default;
			ShowControlsCmd& operator=(ShowControlsCmd&& other) noexcept = default;

			virtual Core::Command* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
