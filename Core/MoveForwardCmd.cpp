#include "pch.h"
#include "MoveForwardCmd.h"
#include "MoveRMC.h"

using namespace Ion;

Core::MoveForwardCmd::MoveForwardCmd(Core::MoveRMC* pMoveRMC)
	: Core::MoveCommand(pMoveRMC)
{
}

Core::MoveCommand* Core::MoveForwardCmd::Duplicate()
{
	return new Core::MoveForwardCmd{ mpMoveRMC };
}

void Core::MoveForwardCmd::Execute()
{
	mpMoveRMC->MoveForward(mLongLongValue);
}
