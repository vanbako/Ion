#include "pch.h"
#include "ToggleSceneTwoCmd.h"
#include "ControlZeroRMC.h"

using namespace Ion;

Core::ToggleSceneTwoCmd::ToggleSceneTwoCmd(Core::ControlZeroRMC* pControlZeroRMC)
	: Core::ControlZeroCommand(pControlZeroRMC)
{
}

Core::ControlZeroCommand* Core::ToggleSceneTwoCmd::Duplicate()
{
	return new Core::ToggleSceneTwoCmd{ mpControlZeroRMC };
}

void Core::ToggleSceneTwoCmd::Execute()
{
	mpControlZeroRMC->ToggleSceneTwo(mLongLongValue);
}
