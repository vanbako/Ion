#include "pch.h"
#include "MoveLeftCmd.h"
#include "ReceiverMC.h"

using namespace Ion::Core;

MoveLeftCmd::MoveLeftCmd(ReceiverMC* pReceiver)
	: Command(pReceiver)
{
}

Command* MoveLeftCmd::Duplicate()
{
	return new MoveLeftCmd{ mpReceiver };
}

void MoveLeftCmd::Execute()
{
	mpReceiver->MoveLeft();
}
