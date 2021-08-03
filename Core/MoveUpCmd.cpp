#include "pch.h"
#include "MoveUpCmd.h"
#include "MoveRMC.h"

using namespace Ion;

Core::MoveUpCmd::MoveUpCmd(Core::MoveRMC* pMoveRMC)
	: Core::MoveCommand(pMoveRMC)
{
}

Core::MoveCommand* Core::MoveUpCmd::Duplicate()
{
	return new Core::MoveUpCmd{ mpMoveRMC };
}

void Core::MoveUpCmd::Execute()
{
	mpMoveRMC->MoveUp(mLongLongValue);
}
