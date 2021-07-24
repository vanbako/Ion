#pragma once
#include "Command.h"

namespace Ion
{
	namespace Core
	{
		class ReceiverMC;

		class MoveBackCmd
			: public Core::Command
		{
		public:
			explicit MoveBackCmd(Core::ReceiverMC* pReceiver);
			virtual ~MoveBackCmd() = default;
			MoveBackCmd(const MoveBackCmd& other) = default;
			MoveBackCmd(MoveBackCmd&& other) noexcept = default;
			MoveBackCmd& operator=(const MoveBackCmd& other) = default;
			MoveBackCmd& operator=(MoveBackCmd&& other) noexcept = default;

			virtual Core::Command* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
