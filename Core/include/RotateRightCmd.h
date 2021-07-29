#pragma once
#include "MoveCommand.h"

namespace Ion
{
	namespace Core
	{
		class RotateRightCmd final
			: public Core::MoveCommand
		{
		public:
			explicit RotateRightCmd(Core::MoveRMC* pMoveRMC);
			virtual ~RotateRightCmd() = default;
			RotateRightCmd(const RotateRightCmd& other) = default;
			RotateRightCmd(RotateRightCmd&& other) noexcept = default;
			RotateRightCmd& operator=(const RotateRightCmd& other) = default;
			RotateRightCmd& operator=(RotateRightCmd&& other) noexcept = default;

			virtual Core::MoveCommand* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
