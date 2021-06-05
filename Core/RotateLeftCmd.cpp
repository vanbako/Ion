#include "../Core/pch.h"
#include "../Core/RotateLeftCmd.h"
#include "../Core/ReceiverMC.h"

using namespace Ion::Core;

RotateLeftCmd::RotateLeftCmd(ReceiverMC* pReceiver)
	: Command(pReceiver)
{
}

Command* RotateLeftCmd::Duplicate()
{
	return new RotateLeftCmd{ mpReceiver };
}

void RotateLeftCmd::Execute()
{
	mpReceiver->RotateLeft();
}
