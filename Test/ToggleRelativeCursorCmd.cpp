#include "pch.h"
#include "../Test/include/ToggleRelativeCursorCmd.h"
#include "../Test/include/ControlZeroRMC.h"

using namespace Ion;

Core::ToggleRelativeCursorCmd::ToggleRelativeCursorCmd(Core::ControlZeroRMC* pControlZeroRMC)
	: Core::ControlZeroCommand(pControlZeroRMC)
{
}

Core::ControlZeroCommand* Core::ToggleRelativeCursorCmd::Duplicate()
{
	return new Core::ToggleRelativeCursorCmd{ mpControlZeroRMC };
}

void Core::ToggleRelativeCursorCmd::Execute()
{
	mpControlZeroRMC->ToggleCursorMode(mLongLongValue);
}
