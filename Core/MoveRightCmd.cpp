#include "pch.h"
#include "MoveRightCmd.h"
#include "MoveRMC.h"

using namespace Ion;

Core::MoveRightCmd::MoveRightCmd(Core::MoveRMC* pMoveRMC)
	: Core::MoveCommand(pMoveRMC)
{
}

Core::MoveCommand* Core::MoveRightCmd::Duplicate()
{
	return new Core::MoveRightCmd{ mpMoveRMC };
}

void Core::MoveRightCmd::Execute()
{
	mpMoveRMC->MoveRight(mLongLongValue);
}
