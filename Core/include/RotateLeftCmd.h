#pragma once
#include "Command.h"

namespace Ion
{
	namespace Core
	{
		class ReceiverMC;

		class RotateLeftCmd final
			: public Core::Command
		{
		public:
			explicit RotateLeftCmd(Core::ReceiverMC* pReceiver);
			virtual ~RotateLeftCmd() = default;
			RotateLeftCmd(const RotateLeftCmd& other) = default;
			RotateLeftCmd(RotateLeftCmd&& other) noexcept = default;
			RotateLeftCmd& operator=(const RotateLeftCmd& other) = default;
			RotateLeftCmd& operator=(RotateLeftCmd&& other) noexcept = default;

			virtual Core::Command* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
