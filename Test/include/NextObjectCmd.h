#pragma once
#include "../Test/include/ControlOneCommand.h"

namespace Ion
{
	namespace Core
	{
		class NextObjectCmd final
			: public Core::ControlOneCommand
		{
		public:
			explicit NextObjectCmd(Core::ControlOneRMC* pControlOneRMC);
			virtual ~NextObjectCmd() = default;
			NextObjectCmd(const NextObjectCmd& other) = default;
			NextObjectCmd(NextObjectCmd&& other) noexcept = default;
			NextObjectCmd& operator=(const NextObjectCmd& other) = default;
			NextObjectCmd& operator=(NextObjectCmd&& other) noexcept = default;

			virtual Core::ControlOneCommand* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
