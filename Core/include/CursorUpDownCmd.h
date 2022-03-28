#pragma once
#include "MoveCommand.h"

namespace Ion
{
	namespace Core
	{
		class CursorUpDownCmd final
			: public Core::MoveCommand
		{
		public:
			explicit CursorUpDownCmd(Core::MoveRMC* pMoveRMC);
			virtual ~CursorUpDownCmd() = default;
			CursorUpDownCmd(const CursorUpDownCmd& other) = default;
			CursorUpDownCmd(CursorUpDownCmd&& other) noexcept = default;
			CursorUpDownCmd& operator=(const CursorUpDownCmd& other) = default;
			CursorUpDownCmd& operator=(CursorUpDownCmd&& other) noexcept = default;

			virtual Core::MoveCommand* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
