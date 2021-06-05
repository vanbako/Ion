#include "../Core/pch.h"
#include "../Core/MoveDownCmd.h"
#include "../Core/ReceiverMC.h"

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
