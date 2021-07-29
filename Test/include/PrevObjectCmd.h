#pragma once
#include "ControlCommand.h"

namespace Ion
{
	namespace Core
	{
		class PrevObjectCmd final
			: public Core::ControlCommand
		{
		public:
			explicit PrevObjectCmd(Core::ControlRMC* pControlRMC);
			virtual ~PrevObjectCmd() = default;
			PrevObjectCmd(const PrevObjectCmd& other) = default;
			PrevObjectCmd(PrevObjectCmd&& other) noexcept = default;
			PrevObjectCmd& operator=(const PrevObjectCmd& other) = default;
			PrevObjectCmd& operator=(PrevObjectCmd&& other) noexcept = default;

			virtual Core::ControlCommand* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
