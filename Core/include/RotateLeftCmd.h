#pragma once
#include "MoveCommand.h"

namespace Ion
{
	namespace Core
	{
		class RotateLeftCmd final
			: public Core::MoveCommand
		{
		public:
			explicit RotateLeftCmd(Core::MoveRMC* pMoveRMC);
			virtual ~RotateLeftCmd() = default;
			RotateLeftCmd(const RotateLeftCmd& other) = default;
			RotateLeftCmd(RotateLeftCmd&& other) noexcept = default;
			RotateLeftCmd& operator=(const RotateLeftCmd& other) = default;
			RotateLeftCmd& operator=(RotateLeftCmd&& other) noexcept = default;

			virtual Core::MoveCommand* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
