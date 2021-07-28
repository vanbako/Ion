#pragma once
#include "Command.h"

namespace Ion
{
	namespace Core
	{
		class ReceiverMC;

		class NextObjectCmd final
			: public Core::Command
		{
		public:
			explicit NextObjectCmd(Core::ReceiverMC* pReceiver);
			virtual ~NextObjectCmd() = default;
			NextObjectCmd(const NextObjectCmd& other) = default;
			NextObjectCmd(NextObjectCmd&& other) noexcept = default;
			NextObjectCmd& operator=(const NextObjectCmd& other) = default;
			NextObjectCmd& operator=(NextObjectCmd&& other) noexcept = default;

			virtual Core::Command* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
