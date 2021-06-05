#include "../Core/pch.h"
#include "../Core/MoveLeftCmd.h"
#include "../Core/ReceiverMC.h"

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
