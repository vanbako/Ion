#include "pch.h"
#include "MoveForwardCmd.h"
#include "ReceiverMC.h"

using namespace Ion::Core;

MoveForwardCmd::MoveForwardCmd(ReceiverMC* pReceiver)
	: Command(pReceiver)
{
}

Command* MoveForwardCmd::Duplicate()
{
	return new MoveForwardCmd{ mpReceiver };
}

void MoveForwardCmd::Execute()
{
	mpReceiver->MoveForward();
}
