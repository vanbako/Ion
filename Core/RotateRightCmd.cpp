#include "pch.h"
#include "RotateRightCmd.h"
#include "MoveRMC.h"

using namespace Ion;

Core::RotateRightCmd::RotateRightCmd(Core::MoveRMC* pMoveRMC)
	: Core::MoveCommand(pMoveRMC)
{
}

Core::MoveCommand* Core::RotateRightCmd::Duplicate()
{
	return new Core::RotateRightCmd{ mpMoveRMC };
}

void Core::RotateRightCmd::Execute()
{
	mpMoveRMC->RotateRight(mLongLongValue);
}
