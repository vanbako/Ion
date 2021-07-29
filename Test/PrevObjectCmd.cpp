#include "pch.h"
#include "PrevObjectCmd.h"
#include "ControlRMC.h"

using namespace Ion;

Core::PrevObjectCmd::PrevObjectCmd(Core::ControlRMC* pControlRMC)
	: Core::ControlCommand(pControlRMC)
{
}

Core::ControlCommand* Core::PrevObjectCmd::Duplicate()
{
	return new Core::PrevObjectCmd{ mpControlRMC };
}

void Core::PrevObjectCmd::Execute()
{
	mpControlRMC->PrevObject(mValue);
}
