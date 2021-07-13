#include "pch.h"
#include "MoveRightCmd.h"
#include "ReceiverMC.h"

using namespace Ion::Core;

MoveRightCmd::MoveRightCmd(ReceiverMC* pReceiver)
	: Command(pReceiver)
{
}

Command* MoveRightCmd::Duplicate()
{
	return new MoveRightCmd{ mpReceiver };
}

void MoveRightCmd::Execute()
{
	mpReceiver->MoveRight();
}
