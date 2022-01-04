#pragma once
#include "../Core/include/Command.h"

namespace Ion
{
	namespace Core
	{
		class ControlZeroRMC;

		class ControlZeroCommand
			: public Core::Command
		{
		public:
			virtual ~ControlZeroCommand() = default;
			ControlZeroCommand(const ControlZeroCommand& other) = default;
			ControlZeroCommand(ControlZeroCommand&& other) noexcept = default;
			ControlZeroCommand& operator=(const ControlZeroCommand& other) = default;
			ControlZeroCommand& operator=(ControlZeroCommand&& other) noexcept = default;

			virtual Core::ControlZeroCommand* Duplicate() = 0;
			virtual void Execute() = 0;
			virtual void SetValue(long long value);
		protected:
			explicit ControlZeroCommand(Core::ControlZeroRMC* pControlZeroRMC);

			Core::ControlZeroRMC* mpControlZeroRMC;
		};
	}
}
