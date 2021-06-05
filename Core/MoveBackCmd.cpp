#include "../Core/pch.h"
#include "../Core/MoveBackCmd.h"
#include "../Core/ReceiverMC.h"

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
