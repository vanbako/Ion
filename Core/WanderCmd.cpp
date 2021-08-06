#include "pch.h"
#include "WanderCmd.h"
#include "SteeringRMC.h"

using namespace Ion;

Core::WanderCmd::WanderCmd(Core::SteeringRMC* pSteeringRMC)
	: SteeringCommand(pSteeringRMC)
{
}

Core::WanderCmd* Core::WanderCmd::Duplicate()
{
	return nullptr;
}

void Core::WanderCmd::Execute()
{
	mpSteeringRMC->Wander(mFloatValue);
}
