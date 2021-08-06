#include "pch.h"
#include "SeekCmd.h"
#include "SteeringRMC.h"

using namespace Ion;

Core::SeekCmd::SeekCmd(Core::SteeringRMC* pSteeringRMC)
	: SteeringCommand(pSteeringRMC)
{
}

Core::SeekCmd* Core::SeekCmd::Duplicate()
{
	return nullptr;
}

void Core::SeekCmd::Execute()
{
	mpSteeringRMC->Seek(mFloatValue);
}
