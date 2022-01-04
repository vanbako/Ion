#include "pch.h"
#include "../Test/include/ToggleSceneOneCmd.h"
#include "../Test/include/ControlZeroRMC.h"

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
