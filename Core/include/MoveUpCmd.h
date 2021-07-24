#pragma once
#include "Command.h"

namespace Ion
{
	namespace Core
	{
		class ReceiverMC;

		class MoveUpCmd
			: public Core::Command
		{
		public:
			explicit MoveUpCmd(Core::ReceiverMC* pReceiver);
			virtual ~MoveUpCmd() = default;
			MoveUpCmd(const MoveUpCmd& other) = default;
			MoveUpCmd(MoveUpCmd&& other) noexcept = default;
			MoveUpCmd& operator=(const MoveUpCmd& other) = default;
			MoveUpCmd& operator=(MoveUpCmd&& other) noexcept = default;

			virtual Core::Command* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
