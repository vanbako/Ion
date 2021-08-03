#include "pch.h"
#include "ShowControlsCmd.h"
#include "ControlRMC.h"

using namespace Ion;

Core::ShowControlsCmd::ShowControlsCmd(Core::ControlRMC* pControlRMC)
	: Core::ControlCommand(pControlRMC)
{
}

Core::ControlCommand* Core::ShowControlsCmd::Duplicate()
{
	return new Core::ShowControlsCmd{ mpControlRMC };
}

void Core::ShowControlsCmd::Execute()
{
	mpControlRMC->ShowControls(mLongLongValue);
}
