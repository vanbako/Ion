#pragma once
#include "Command.h"

namespace Ion
{
	namespace Core
	{
		class ReceiverMC;

		class MoveLeftCmd
			: public Command
		{
		public:
			explicit MoveLeftCmd(ReceiverMC* pReceiver);
			virtual ~MoveLeftCmd() = default;
			MoveLeftCmd(const MoveLeftCmd& other) = default;
			MoveLeftCmd(MoveLeftCmd&& other) noexcept = default;
			MoveLeftCmd& operator=(const MoveLeftCmd& other) = default;
			MoveLeftCmd& operator=(MoveLeftCmd&& other) noexcept = default;

			virtual Command* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
