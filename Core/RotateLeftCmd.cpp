#include "pch.h"
#include "RotateLeftCmd.h"
#include "MoveRMC.h"

using namespace Ion;

Core::RotateLeftCmd::RotateLeftCmd(Core::MoveRMC* pMoveRMC)
	: Core::MoveCommand(pMoveRMC)
{
}

Core::MoveCommand* Core::RotateLeftCmd::Duplicate()
{
	return new Core::RotateLeftCmd{ mpMoveRMC };
}

void Core::RotateLeftCmd::Execute()
{
	mpMoveRMC->RotateLeft(mValue);
}
