#include "../Core/pch.h"
#include "../Core/RotateRightCmd.h"
#include "../Core/ReceiverMC.h"

using namespace Ion::Core;

RotateRightCmd::RotateRightCmd(ReceiverMC* pReceiver)
	: Command(pReceiver)
{
}

Command* RotateRightCmd::Duplicate()
{
	return new RotateRightCmd{ mpReceiver };
}

void RotateRightCmd::Execute()
{
	mpReceiver->RotateRight();
}
