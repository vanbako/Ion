#pragma once
#include "../Core/Command.h"

namespace Ion
{
	namespace Core
	{
		class ReceiverMC;

		class MoveBackCmd
			: public Command
		{
		public:
			explicit MoveBackCmd(ReceiverMC* pReceiver);
			virtual ~MoveBackCmd() = default;
			MoveBackCmd(const MoveBackCmd& other) = default;
			MoveBackCmd(MoveBackCmd&& other) noexcept = default;
			MoveBackCmd& operator=(const MoveBackCmd& other) = default;
			MoveBackCmd& operator=(MoveBackCmd&& other) noexcept = default;

			virtual Command* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
