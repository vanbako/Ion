#include "../Core/pch.h"
#include "../Core/MoveForwardCmd.h"
#include "../Core/ReceiverMC.h"

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
