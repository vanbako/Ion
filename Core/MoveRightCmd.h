#pragma once
#include "../Core/Command.h"

namespace Ion
{
	namespace Core
	{
		class ReceiverMC;

		class MoveRightCmd
			: public Command
		{
		public:
			explicit MoveRightCmd(ReceiverMC* pReceiver);
			virtual ~MoveRightCmd() = default;
			MoveRightCmd(const MoveRightCmd& other) = default;
			MoveRightCmd(MoveRightCmd&& other) noexcept = default;
			MoveRightCmd& operator=(const MoveRightCmd& other) = default;
			MoveRightCmd& operator=(MoveRightCmd&& other) noexcept = default;

			virtual Command* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
