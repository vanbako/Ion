#include "pch.h"
#include "RotateLeftCmd.h"
#include "ReceiverMC.h"

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
