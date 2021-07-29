#pragma once
#include "MoveCommand.h"

namespace Ion
{
	namespace Core
	{
		class MoveDownCmd final
			: public Core::MoveCommand
		{
		public:
			explicit MoveDownCmd(Core::MoveRMC* pMoveRMC);
			virtual ~MoveDownCmd() = default;
			MoveDownCmd(const MoveDownCmd& other) = default;
			MoveDownCmd(MoveDownCmd&& other) noexcept = default;
			MoveDownCmd& operator=(const MoveDownCmd& other) = default;
			MoveDownCmd& operator=(MoveDownCmd&& other) noexcept = default;

			virtual Core::MoveCommand* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
