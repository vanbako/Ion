#pragma once
#include "Command.h"

namespace Ion
{
	namespace Core
	{
		class ReceiverMC;

		class MoveRightCmd final
			: public Core::Command
		{
		public:
			explicit MoveRightCmd(Core::ReceiverMC* pReceiver);
			virtual ~MoveRightCmd() = default;
			MoveRightCmd(const MoveRightCmd& other) = default;
			MoveRightCmd(MoveRightCmd&& other) noexcept = default;
			MoveRightCmd& operator=(const MoveRightCmd& other) = default;
			MoveRightCmd& operator=(MoveRightCmd&& other) noexcept = default;

			virtual Core::Command* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
