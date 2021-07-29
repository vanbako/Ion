#pragma once
#include "MoveCommand.h"

namespace Ion
{
	namespace Core
	{
		class MoveForwardCmd final
			: public Core::MoveCommand
		{
		public:
			explicit MoveForwardCmd(Core::MoveRMC* pMoveRMC);
			virtual ~MoveForwardCmd() = default;
			MoveForwardCmd(const MoveForwardCmd& other) = default;
			MoveForwardCmd(MoveForwardCmd&& other) noexcept = default;
			MoveForwardCmd& operator=(const MoveForwardCmd& other) = default;
			MoveForwardCmd& operator=(MoveForwardCmd&& other) noexcept = default;

			virtual Core::MoveCommand* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
