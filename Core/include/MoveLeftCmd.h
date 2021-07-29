#pragma once
#include "MoveCommand.h"

namespace Ion
{
	namespace Core
	{
		class MoveLeftCmd final
			: public Core::MoveCommand
		{
		public:
			explicit MoveLeftCmd(Core::MoveRMC* pMoveRMC);
			virtual ~MoveLeftCmd() = default;
			MoveLeftCmd(const MoveLeftCmd& other) = default;
			MoveLeftCmd(MoveLeftCmd&& other) noexcept = default;
			MoveLeftCmd& operator=(const MoveLeftCmd& other) = default;
			MoveLeftCmd& operator=(MoveLeftCmd&& other) noexcept = default;

			virtual Core::MoveCommand* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
