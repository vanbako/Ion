#include "pch.h"
#include "ShowControlsCmd.h"
#include "ReceiverMC.h"

using namespace Ion;

Core::ShowControlsCmd::ShowControlsCmd(Core::ReceiverMC* pReceiver)
	: Core::Command(pReceiver)
{
}

Core::Command* Core::ShowControlsCmd::Duplicate()
{
	return new Core::ShowControlsCmd{ mpReceiver };
}

void Core::ShowControlsCmd::Execute()
{
	mpReceiver->ShowControls(mValue);
}
