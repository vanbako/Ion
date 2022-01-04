#include "pch.h"
#include "../Test/include/ControlOneCommand.h"
#include "../Test/include/ControlOneRMC.h"

using namespace Ion;

Core::ControlOneCommand::ControlOneCommand(Core::ControlOneRMC* pControlOneRMC)
	: Command()
	, mpControlOneRMC{ pControlOneRMC }
{
}

void Core::ControlOneCommand::SetValue(long long value)
{
	mLongLongValue = value;
}
