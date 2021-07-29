#include "pch.h"
#include "MoveDownCmd.h"
#include "MoveRMC.h"

using namespace Ion;

Core::MoveDownCmd::MoveDownCmd(Core::MoveRMC* pMoveRMC)
	: Core::MoveCommand(pMoveRMC)
{
}

Core::MoveCommand* Core::MoveDownCmd::Duplicate()
{
	return new Core::MoveDownCmd{ mpMoveRMC };
}

void Core::MoveDownCmd::Execute()
{
	mpMoveRMC->MoveDown(mValue);
}
