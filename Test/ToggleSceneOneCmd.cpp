#include "pch.h"
#include "ToggleSceneOneCmd.h"
#include "ControlZeroRMC.h"

using namespace Ion;

Core::ToggleSceneOneCmd::ToggleSceneOneCmd(Core::ControlZeroRMC* pControlZeroRMC)
	: Core::ControlZeroCommand(pControlZeroRMC)
{
}

Core::ControlZeroCommand* Core::ToggleSceneOneCmd::Duplicate()
{
	return new Core::ToggleSceneOneCmd{ mpControlZeroRMC };
}

void Core::ToggleSceneOneCmd::Execute()
{
	mpControlZeroRMC->ToggleSceneOne(mLongLongValue);
}
