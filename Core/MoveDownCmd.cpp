#include "pch.h"
#include "MoveDownCmd.h"
#include "ReceiverMC.h"

using namespace Ion;

Core::MoveDownCmd::MoveDownCmd(Core::ReceiverMC* pReceiver)
	: Core::Command(pReceiver)
{
}

Core::Command* Core::MoveDownCmd::Duplicate()
{
	return new Core::MoveDownCmd{ mpReceiver };
}

void Core::MoveDownCmd::Execute()
{
	mpReceiver->MoveDown();
}
