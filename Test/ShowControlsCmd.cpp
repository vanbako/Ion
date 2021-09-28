#include "pch.h"
#include "ShowControlsCmd.h"
#include "ControlOneRMC.h"

using namespace Ion;

Core::ShowControlsCmd::ShowControlsCmd(Core::ControlOneRMC* pControlOneRMC)
	: Core::ControlOneCommand(pControlOneRMC)
{
}

Core::ControlOneCommand* Core::ShowControlsCmd::Duplicate()
{
	return new Core::ShowControlsCmd{ mpControlOneRMC };
}

void Core::ShowControlsCmd::Execute()
{
	mpControlOneRMC->ShowControls(mLongLongValue);
}
