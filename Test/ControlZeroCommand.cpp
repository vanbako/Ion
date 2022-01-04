#include "pch.h"
#include "../Test/include/ControlZeroCommand.h"
#include "../Test/include/ControlZeroRMC.h"

using namespace Ion;

Core::ControlZeroCommand::ControlZeroCommand(Core::ControlZeroRMC* pControlZeroRMC)
	: Command()
	, mpControlZeroRMC{ pControlZeroRMC }
{
}

void Core::ControlZeroCommand::SetValue(long long value)
{
	mLongLongValue = value;
}
