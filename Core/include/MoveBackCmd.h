#pragma once
#include "MoveCommand.h"

namespace Ion
{
	namespace Core
	{
		class MoveBackCmd final
			: public Core::MoveCommand
		{
		public:
			explicit MoveBackCmd(Core::MoveRMC* pMoveRMC);
			virtual ~MoveBackCmd() = default;
			MoveBackCmd(const MoveBackCmd& other) = default;
			MoveBackCmd(MoveBackCmd&& other) noexcept = default;
			MoveBackCmd& operator=(const MoveBackCmd& other) = default;
			MoveBackCmd& operator=(MoveBackCmd&& other) noexcept = default;

			virtual Core::MoveCommand* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
