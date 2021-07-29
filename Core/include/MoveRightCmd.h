#pragma once
#include "MoveCommand.h"

namespace Ion
{
	namespace Core
	{
		class MoveRightCmd final
			: public Core::MoveCommand
		{
		public:
			explicit MoveRightCmd(Core::MoveRMC* pMoveRMC);
			virtual ~MoveRightCmd() = default;
			MoveRightCmd(const MoveRightCmd& other) = default;
			MoveRightCmd(MoveRightCmd&& other) noexcept = default;
			MoveRightCmd& operator=(const MoveRightCmd& other) = default;
			MoveRightCmd& operator=(MoveRightCmd&& other) noexcept = default;

			virtual Core::MoveCommand* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
