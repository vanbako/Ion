#include "pch.h"
#include "MoveLeftCmd.h"
#include "MoveRMC.h"

using namespace Ion;

Core::MoveLeftCmd::MoveLeftCmd(Core::MoveRMC* pMoveRMC)
	: Core::MoveCommand(pMoveRMC)
{
}

Core::MoveCommand* Core::MoveLeftCmd::Duplicate()
{
	return new Core::MoveLeftCmd{ mpMoveRMC };
}

void Core::MoveLeftCmd::Execute()
{
	mpMoveRMC->MoveLeft(mLongLongValue);
}
