#pragma once
#include "ControlCommand.h"

namespace Ion
{
	namespace Core
	{
		class NextObjectCmd final
			: public Core::ControlCommand
		{
		public:
			explicit NextObjectCmd(Core::ControlRMC* pControlRMC);
			virtual ~NextObjectCmd() = default;
			NextObjectCmd(const NextObjectCmd& other) = default;
			NextObjectCmd(NextObjectCmd&& other) noexcept = default;
			NextObjectCmd& operator=(const NextObjectCmd& other) = default;
			NextObjectCmd& operator=(NextObjectCmd&& other) noexcept = default;

			virtual Core::ControlCommand* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
