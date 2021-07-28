#pragma once
#include "Command.h"

namespace Ion
{
	namespace Core
	{
		class ReceiverMC;

		class MoveLeftCmd final
			: public Core::Command
		{
		public:
			explicit MoveLeftCmd(Core::ReceiverMC* pReceiver);
			virtual ~MoveLeftCmd() = default;
			MoveLeftCmd(const MoveLeftCmd& other) = default;
			MoveLeftCmd(MoveLeftCmd&& other) noexcept = default;
			MoveLeftCmd& operator=(const MoveLeftCmd& other) = default;
			MoveLeftCmd& operator=(MoveLeftCmd&& other) noexcept = default;

			virtual Core::Command* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
