#pragma once
#include "Command.h"

namespace Ion
{
	namespace Core
	{
		class ReceiverMC;

		class MoveForwardCmd
			: public Command
		{
		public:
			explicit MoveForwardCmd(ReceiverMC* pReceiver);
			virtual ~MoveForwardCmd() = default;
			MoveForwardCmd(const MoveForwardCmd& other) = default;
			MoveForwardCmd(MoveForwardCmd&& other) noexcept = default;
			MoveForwardCmd& operator=(const MoveForwardCmd& other) = default;
			MoveForwardCmd& operator=(MoveForwardCmd&& other) noexcept = default;

			virtual Command* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
