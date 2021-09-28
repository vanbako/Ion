#include "pch.h"
#include "ControlZeroCommand.h"
#include "ControlZeroRMC.h"

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
