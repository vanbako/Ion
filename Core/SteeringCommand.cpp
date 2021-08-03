#include "pch.h"
#include "SteeringCommand.h"
#include "SteeringRMC.h"

using namespace Ion;

Core::SteeringCommand::SteeringCommand(Core::SteeringRMC* pSteeringRMC)
	: Command()
	, mpSteeringRMC{ pSteeringRMC }
{
}
