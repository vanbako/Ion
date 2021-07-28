#pragma once
#include "Command.h"

namespace Ion
{
	namespace Core
	{
		class ReceiverMC;

		class PrevObjectCmd final
			: public Core::Command
		{
		public:
			explicit PrevObjectCmd(Core::ReceiverMC* pReceiver);
			virtual ~PrevObjectCmd() = default;
			PrevObjectCmd(const PrevObjectCmd& other) = default;
			PrevObjectCmd(PrevObjectCmd&& other) noexcept = default;
			PrevObjectCmd& operator=(const PrevObjectCmd& other) = default;
			PrevObjectCmd& operator=(PrevObjectCmd&& other) noexcept = default;

			virtual Core::Command* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
