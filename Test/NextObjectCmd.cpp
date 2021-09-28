#include "pch.h"
#include "NextObjectCmd.h"
#include "ControlOneRMC.h"

using namespace Ion;

Core::NextObjectCmd::NextObjectCmd(Core::ControlOneRMC* pControlOneRMC)
	: Core::ControlOneCommand(pControlOneRMC)
{
}

Core::ControlOneCommand* Core::NextObjectCmd::Duplicate()
{
	return new Core::NextObjectCmd{ mpControlOneRMC };
}

void Core::NextObjectCmd::Execute()
{
	mpControlOneRMC->NextObject(mLongLongValue);
}
