#include "pch.h"
#include "MoveLeftCmd.h"
#include "ReceiverMC.h"

using namespace Ion;

Core::MoveLeftCmd::MoveLeftCmd(Core::ReceiverMC* pReceiver)
	: Core::Command(pReceiver)
{
}

Core::Command* Core::MoveLeftCmd::Duplicate()
{
	return new Core::MoveLeftCmd{ mpReceiver };
}

void Core::MoveLeftCmd::Execute()
{
	mpReceiver->MoveLeft();
}
