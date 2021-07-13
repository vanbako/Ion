#include "pch.h"
#include "MoveBackCmd.h"
#include "ReceiverMC.h"

using namespace Ion::Core;

MoveBackCmd::MoveBackCmd(ReceiverMC* pReceiver)
	: Command(pReceiver)
{
}

Command* MoveBackCmd::Duplicate()
{
	return new MoveBackCmd{ mpReceiver };
}

void MoveBackCmd::Execute()
{
	mpReceiver->MoveBack();
}
