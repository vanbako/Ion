#include "pch.h"
#include "MoveUpCmd.h"
#include "ReceiverMC.h"

using namespace Ion::Core;

MoveUpCmd::MoveUpCmd(ReceiverMC* pReceiver)
	: Command(pReceiver)
{
}

Command* MoveUpCmd::Duplicate()
{
	return new MoveUpCmd{ mpReceiver };
}

void MoveUpCmd::Execute()
{
	mpReceiver->MoveUp();
}
