#include "pch.h"
#include "CursorRotateLeftRightCmd.h"
#include "MoveRMC.h"

using namespace Ion;

Core::CursorRotateLeftRightCmd::CursorRotateLeftRightCmd(Core::MoveRMC* pMoveRMC)
	: Core::MoveCommand(pMoveRMC)
{
}

Core::MoveCommand* Core::CursorRotateLeftRightCmd::Duplicate()
{
	return new Core::CursorRotateLeftRightCmd{ mpMoveRMC };
}

void Core::CursorRotateLeftRightCmd::Execute()
{
	mpMoveRMC->CursorRotateLeftRight(mIntValue);
}
