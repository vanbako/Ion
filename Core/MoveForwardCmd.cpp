#include "pch.h"
#include "MoveForwardCmd.h"
#include "ReceiverMC.h"

using namespace Ion;

Core::MoveForwardCmd::MoveForwardCmd(Core::ReceiverMC* pReceiver)
	: Core::Command(pReceiver)
{
}

Core::Command* Core::MoveForwardCmd::Duplicate()
{
	return new Core::MoveForwardCmd{ mpReceiver };
}

void Core::MoveForwardCmd::Execute()
{
	mpReceiver->MoveForward();
}
