#include "pch.h"
#include "PrevObjectCmd.h"
#include "ControlOneRMC.h"

using namespace Ion;

Core::PrevObjectCmd::PrevObjectCmd(Core::ControlOneRMC* pControlOneRMC)
	: Core::ControlOneCommand(pControlOneRMC)
{
}

Core::ControlOneCommand* Core::PrevObjectCmd::Duplicate()
{
	return new Core::PrevObjectCmd{ mpControlOneRMC };
}

void Core::PrevObjectCmd::Execute()
{
	mpControlOneRMC->PrevObject(mLongLongValue);
}
