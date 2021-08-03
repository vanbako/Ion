#include "pch.h"
#include "MoveCommand.h"
#include "MoveRMC.h"

using namespace Ion;

Core::MoveCommand::MoveCommand(Core::MoveRMC* pMoveRMC)
	: Command()
	, mpMoveRMC{ pMoveRMC }
{
}
