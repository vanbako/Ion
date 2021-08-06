#pragma once
#include "SteeringCommand.h"

namespace Ion
{
	namespace Core
	{
		class SeekCmd final
			: public Core::SteeringCommand
		{
		public:
			explicit SeekCmd(Core::SteeringRMC* pSteeringRMC);
			virtual ~SeekCmd() = default;
			SeekCmd(const SeekCmd& other) = default;
			SeekCmd(SeekCmd&& other) noexcept = default;
			SeekCmd& operator=(const SeekCmd& other) = default;
			SeekCmd& operator=(SeekCmd&& other) noexcept = default;

			virtual Core::SeekCmd* Duplicate() override;
			virtual void Execute() override;
		};
	}
}
