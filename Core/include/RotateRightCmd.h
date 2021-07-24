#pragma once
#include "Command.h"

namespace Ion
{
	namespace Core
	{
		class ReceiverMC;

		class RotateRightCmd
			: public Core::Command
		{
		public:
			explicit RotateRightCmd(Core::ReceiverMC* pReceiver);
			virtual ~RotateRightCmd() = default;
			RotateRightCmd(const RotateRightCmd& other) = default;
			RotateRightCmd(RotateRightCmd&& other) noexcept = default;
			RotateRightCmd& operator=(const RotateRightCmd& other) = default;
			RotateRightCmd& operator=(RotateRightCmd&& other) noexcept = default;

			virtual Core::Command* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
