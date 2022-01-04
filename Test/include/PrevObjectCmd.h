#pragma once
#include "../Test/include/ControlOneCommand.h"

namespace Ion
{
	namespace Core
	{
		class PrevObjectCmd final
			: public Core::ControlOneCommand
		{
		public:
			explicit PrevObjectCmd(Core::ControlOneRMC* pControlOneRMC);
			virtual ~PrevObjectCmd() = default;
			PrevObjectCmd(const PrevObjectCmd& other) = default;
			PrevObjectCmd(PrevObjectCmd&& other) noexcept = default;
			PrevObjectCmd& operator=(const PrevObjectCmd& other) = default;
			PrevObjectCmd& operator=(PrevObjectCmd&& other) noexcept = default;

			virtual Core::ControlOneCommand* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
