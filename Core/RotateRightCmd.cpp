#include "pch.h"
#include "RotateRightCmd.h"
#include "ReceiverMC.h"

using namespace Ion;

Core::RotateRightCmd::RotateRightCmd(Core::ReceiverMC* pReceiver)
	: Core::Command(pReceiver)
{
}

Core::Command* Core::RotateRightCmd::Duplicate()
{
	return new Core::RotateRightCmd{ mpReceiver };
}

void Core::RotateRightCmd::Execute()
{
	mpReceiver->RotateRight();
}
