#pragma once
#include "../Core/Command.h"

namespace Ion
{
	namespace Core
	{
		class ReceiverMC;

		class MoveUpCmd
			: public Command
		{
		public:
			explicit MoveUpCmd(ReceiverMC* pReceiver);
			virtual ~MoveUpCmd() = default;
			MoveUpCmd(const MoveUpCmd& other) = default;
			MoveUpCmd(MoveUpCmd&& other) noexcept = default;
			MoveUpCmd& operator=(const MoveUpCmd& other) = default;
			MoveUpCmd& operator=(MoveUpCmd&& other) noexcept = default;

			virtual Command* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
