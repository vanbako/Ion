#pragma once
#include "Command.h"

namespace Ion
{
	namespace Core
	{
		class ReceiverMC;

		class MoveDownCmd
			: public Command
		{
		public:
			explicit MoveDownCmd(ReceiverMC* pReceiver);
			virtual ~MoveDownCmd() = default;
			MoveDownCmd(const MoveDownCmd& other) = default;
			MoveDownCmd(MoveDownCmd&& other) noexcept = default;
			MoveDownCmd& operator=(const MoveDownCmd& other) = default;
			MoveDownCmd& operator=(MoveDownCmd&& other) noexcept = default;

			virtual Command* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
