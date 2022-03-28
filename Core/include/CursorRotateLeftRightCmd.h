#pragma once
#include "MoveCommand.h"

namespace Ion
{
	namespace Core
	{
		class CursorRotateLeftRightCmd final
			: public Core::MoveCommand
		{
		public:
			explicit CursorRotateLeftRightCmd(Core::MoveRMC* pMoveRMC);
			virtual ~CursorRotateLeftRightCmd() = default;
			CursorRotateLeftRightCmd(const CursorRotateLeftRightCmd& other) = default;
			CursorRotateLeftRightCmd(CursorRotateLeftRightCmd&& other) noexcept = default;
			CursorRotateLeftRightCmd& operator=(const CursorRotateLeftRightCmd& other) = default;
			CursorRotateLeftRightCmd& operator=(CursorRotateLeftRightCmd&& other) noexcept = default;

			virtual Core::MoveCommand* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
