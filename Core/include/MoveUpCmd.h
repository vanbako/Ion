#pragma once
#include "MoveCommand.h"

namespace Ion
{
	namespace Core
	{
		class MoveUpCmd final
			: public Core::MoveCommand
		{
		public:
			explicit MoveUpCmd(Core::MoveRMC* pMoveRMC);
			virtual ~MoveUpCmd() = default;
			MoveUpCmd(const MoveUpCmd& other) = default;
			MoveUpCmd(MoveUpCmd&& other) noexcept = default;
			MoveUpCmd& operator=(const MoveUpCmd& other) = default;
			MoveUpCmd& operator=(MoveUpCmd&& other) noexcept = default;

			virtual Core::MoveCommand* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
