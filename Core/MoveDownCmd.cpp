#include "pch.h"
#include "MoveDownCmd.h"
#include "ReceiverMC.h"

using namespace Ion::Core;

MoveDownCmd::MoveDownCmd(ReceiverMC* pReceiver)
	: Command(pReceiver)
{
}

Command* MoveDownCmd::Duplicate()
{
	return new MoveDownCmd{ mpReceiver };
}

void MoveDownCmd::Execute()
{
	mpReceiver->MoveDown();
}
