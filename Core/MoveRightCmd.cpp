#include "pch.h"
#include "MoveRightCmd.h"
#include "ReceiverMC.h"

using namespace Ion;

Core::MoveRightCmd::MoveRightCmd(Core::ReceiverMC* pReceiver)
	: Core::Command(pReceiver)
{
}

Core::Command* Core::MoveRightCmd::Duplicate()
{
	return new Core::MoveRightCmd{ mpReceiver };
}

void Core::MoveRightCmd::Execute()
{
	mpReceiver->MoveRight();
}
