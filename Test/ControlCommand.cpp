#include "pch.h"
#include "ControlCommand.h"
#include "ControlRMC.h"

using namespace Ion;

Core::ControlCommand::ControlCommand(Core::ControlRMC* pControlRMC)
	: Command()
	, mpControlRMC{ pControlRMC }
{
}

void Core::ControlCommand::SetValue(long long value)
{
	mValue = value;
}
