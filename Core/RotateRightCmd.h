#pragma once
#include "../Core/Command.h"

namespace Ion
{
	namespace Core
	{
		class ReceiverMC;

		class RotateRightCmd
			: public Command
		{
		public:
			explicit RotateRightCmd(ReceiverMC* pReceiver);
			virtual ~RotateRightCmd() = default;
			RotateRightCmd(const RotateRightCmd& other) = default;
			RotateRightCmd(RotateRightCmd&& other) noexcept = default;
			RotateRightCmd& operator=(const RotateRightCmd& other) = default;
			RotateRightCmd& operator=(RotateRightCmd&& other) noexcept = default;

			virtual Command* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
