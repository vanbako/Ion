#include "pch.h"
#include "NextObjectCmd.h"
#include "ControlRMC.h"

using namespace Ion;

Core::NextObjectCmd::NextObjectCmd(Core::ControlRMC* pControlRMC)
	: Core::ControlCommand(pControlRMC)
{
}

Core::ControlCommand* Core::NextObjectCmd::Duplicate()
{
	return new Core::NextObjectCmd{ mpControlRMC };
}

void Core::NextObjectCmd::Execute()
{
	mpControlRMC->NextObject(mValue);
}
