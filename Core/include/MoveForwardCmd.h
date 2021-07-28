#pragma once
#include "Command.h"

namespace Ion
{
	namespace Core
	{
		class ReceiverMC;

		class MoveForwardCmd final
			: public Core::Command
		{
		public:
			explicit MoveForwardCmd(Core::ReceiverMC* pReceiver);
			virtual ~MoveForwardCmd() = default;
			MoveForwardCmd(const MoveForwardCmd& other) = default;
			MoveForwardCmd(MoveForwardCmd&& other) noexcept = default;
			MoveForwardCmd& operator=(const MoveForwardCmd& other) = default;
			MoveForwardCmd& operator=(MoveForwardCmd&& other) noexcept = default;

			virtual Core::Command* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
