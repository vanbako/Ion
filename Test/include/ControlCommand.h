#pragma once
#include "Command.h"

namespace Ion
{
	namespace Core
	{
		class ControlRMC;

		class ControlCommand
			: public Core::Command
		{
		public:
			virtual ~ControlCommand() = default;
			ControlCommand(const ControlCommand& other) = default;
			ControlCommand(ControlCommand&& other) noexcept = default;
			ControlCommand& operator=(const ControlCommand& other) = default;
			ControlCommand& operator=(ControlCommand&& other) noexcept = default;

			virtual Core::ControlCommand* Duplicate() = 0;
			virtual void Execute() = 0;
			virtual void SetValue(long long value);
		protected:
			explicit ControlCommand(Core::ControlRMC* pControlRMC);

			Core::ControlRMC* mpControlRMC;
		};
	}
}
