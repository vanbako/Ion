#include "pch.h"
#include "MoveBackCmd.h"
#include "MoveRMC.h"

using namespace Ion;

Core::MoveBackCmd::MoveBackCmd(Core::MoveRMC* pMoveRMC)
	: Core::MoveCommand(pMoveRMC)
{
}

Core::MoveCommand* Core::MoveBackCmd::Duplicate()
{
	return new Core::MoveBackCmd{ mpMoveRMC };
}

void Core::MoveBackCmd::Execute()
{
	mpMoveRMC->MoveBack(mValue);
}
