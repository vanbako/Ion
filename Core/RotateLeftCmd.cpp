#include "pch.h"
#include "RotateLeftCmd.h"
#include "ReceiverMC.h"

using namespace Ion;

Core::RotateLeftCmd::RotateLeftCmd(Core::ReceiverMC* pReceiver)
	: Core::Command(pReceiver)
{
}

Core::Command* Core::RotateLeftCmd::Duplicate()
{
	return new Core::RotateLeftCmd{ mpReceiver };
}

void Core::RotateLeftCmd::Execute()
{
	mpReceiver->RotateLeft();
}
