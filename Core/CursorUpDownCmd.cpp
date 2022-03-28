#include "pch.h"
#include "CursorUpDownCmd.h"
#include "MoveRMC.h"

using namespace Ion;

Core::CursorUpDownCmd::CursorUpDownCmd(Core::MoveRMC* pMoveRMC)
	: Core::MoveCommand(pMoveRMC)
{
}

Core::MoveCommand* Core::CursorUpDownCmd::Duplicate()
{
	return new Core::CursorUpDownCmd{ mpMoveRMC };
}

void Core::CursorUpDownCmd::Execute()
{
	mpMoveRMC->CursorUpDown(mIntValue);
}
