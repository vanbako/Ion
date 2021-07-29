#include "pch.h"
#include "MoveBackCmd.h"
#include "ReceiverMC.h"

using namespace Ion;

Core::MoveBackCmd::MoveBackCmd(Core::ReceiverMC* pReceiver)
	: Core::Command(pReceiver)
{
}

Core::Command* Core::MoveBackCmd::Duplicate()
{
	return new Core::MoveBackCmd{ mpReceiver };
}

void Core::MoveBackCmd::Execute()
{
	mpReceiver->MoveBack(mValue);
}
