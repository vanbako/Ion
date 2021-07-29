#include "pch.h"
#include "PrevObjectCmd.h"
#include "ReceiverMC.h"

using namespace Ion;

Core::PrevObjectCmd::PrevObjectCmd(Core::ReceiverMC* pReceiver)
	: Core::Command(pReceiver)
{
}

Core::Command* Core::PrevObjectCmd::Duplicate()
{
	return new Core::PrevObjectCmd{ mpReceiver };
}

void Core::PrevObjectCmd::Execute()
{
	mpReceiver->PrevObject(mValue);
}
