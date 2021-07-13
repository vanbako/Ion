#pragma once
#include "Command.h"

namespace Ion
{
	namespace Core
	{
		class ReceiverMC;

		class RotateLeftCmd
			: public Command
		{
		public:
			explicit RotateLeftCmd(ReceiverMC* pReceiver);
			virtual ~RotateLeftCmd() = default;
			RotateLeftCmd(const RotateLeftCmd& other) = default;
			RotateLeftCmd(RotateLeftCmd&& other) noexcept = default;
			RotateLeftCmd& operator=(const RotateLeftCmd& other) = default;
			RotateLeftCmd& operator=(RotateLeftCmd&& other) noexcept = default;

			virtual Command* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
