#pragma once
#include "../Core/include/Command.h"

namespace Ion
{
	namespace Core
	{
		class ControlOneRMC;

		class ControlOneCommand
			: public Core::Command
		{
		public:
			virtual ~ControlOneCommand() = default;
			ControlOneCommand(const ControlOneCommand& other) = default;
			ControlOneCommand(ControlOneCommand&& other) noexcept = default;
			ControlOneCommand& operator=(const ControlOneCommand& other) = default;
			ControlOneCommand& operator=(ControlOneCommand&& other) noexcept = default;

			virtual Core::ControlOneCommand* Duplicate() = 0;
			virtual void Execute() = 0;
			virtual void SetValue(long long value);
		protected:
			explicit ControlOneCommand(Core::ControlOneRMC* pControlOneRMC);

			Core::ControlOneRMC* mpControlOneRMC;
		};
	}
}
