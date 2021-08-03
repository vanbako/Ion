#include "pch.h"
#include "BehaviourCC.h"
#include "WanderCmd.h"

using namespace Ion;

Core::BehaviourCC::BehaviourCC(bool isActive, Core::Object* pObject)
	: ControllerC(isActive, pObject)
	, mBehaviour{ Behaviour::Idle }
	, mpSteeringRMC{ nullptr }
	, mpWanderCmd{ nullptr }
{
}

Core::BehaviourCC::~BehaviourCC()
{
	if (mpSteeringRMC != nullptr)
	{
		delete mpWanderCmd;
	}
}

void Core::BehaviourCC::Update(float delta)
{
	if (mpSteeringRMC == nullptr)
		return;
	switch (mBehaviour)
	{
	case Behaviour::Wander:
		mpWanderCmd->SetValue(delta);
		mpWanderCmd->Execute();
		break;
	}
}

void Core::BehaviourCC::SetSteeringRMC(SteeringRMC* pSteeringRMC)
{
	mpSteeringRMC = pSteeringRMC;
	mpWanderCmd = new WanderCmd{ pSteeringRMC };
}

void Core::BehaviourCC::SetBehaviour(Behaviour behaviour)
{
	mBehaviour = behaviour;
}
