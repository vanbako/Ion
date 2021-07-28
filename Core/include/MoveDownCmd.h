#pragma once
#include "Command.h"

namespace Ion
{
	namespace Core
	{
		class ReceiverMC;

		class MoveDownCmd final
			: public Core::Command
		{
		public:
			explicit MoveDownCmd(Core::ReceiverMC* pReceiver);
			virtual ~MoveDownCmd() = default;
			MoveDownCmd(const MoveDownCmd& other) = default;
			MoveDownCmd(MoveDownCmd&& other) noexcept = default;
			MoveDownCmd& operator=(const MoveDownCmd& other) = default;
			MoveDownCmd& operator=(MoveDownCmd&& other) noexcept = default;

			virtual Core::Command* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
