#include "pch.h"
#include "MoveUpCmd.h"
#include "ReceiverMC.h"

using namespace Ion;

Core::MoveUpCmd::MoveUpCmd(Core::ReceiverMC* pReceiver)
	: Core::Command(pReceiver)
{
}

Core::Command* Core::MoveUpCmd::Duplicate()
{
	return new Core::MoveUpCmd{ mpReceiver };
}

void Core::MoveUpCmd::Execute()
{
	mpReceiver->MoveUp();
}
