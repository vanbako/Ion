#pragma once
#include "Command.h"

namespace Ion
{
	namespace Core
	{
		class MoveRMC;

		class MoveCommand
			: public Core::Command
		{
		public:
			virtual ~MoveCommand() = default;
			MoveCommand(const MoveCommand& other) = default;
			MoveCommand(MoveCommand&& other) noexcept = default;
			MoveCommand& operator=(const MoveCommand& other) = default;
			MoveCommand& operator=(MoveCommand&& other) noexcept = default;

			virtual Core::MoveCommand* Duplicate() = 0;
			virtual void Execute() = 0;
		protected:
			explicit MoveCommand(Core::MoveRMC* pMoveRMC);

			Core::MoveRMC* mpMoveRMC;
		};
	}
}
