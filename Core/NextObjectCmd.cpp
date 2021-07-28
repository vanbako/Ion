#include "pch.h"
#include "NextObjectCmd.h"
#include "ReceiverMC.h"

using namespace Ion;

Core::NextObjectCmd::NextObjectCmd(Core::ReceiverMC* pReceiver)
	: Core::Command(pReceiver)
{
}

Core::Command* Core::NextObjectCmd::Duplicate()
{
	return new Core::NextObjectCmd{ mpReceiver };
}

void Core::NextObjectCmd::Execute()
{
	mpReceiver->NextObject();
}
