#include "pch.h"
#include "Application.h"
#include "PhysicsST.h"

using namespace Ion::Core;

PhysicsST::PhysicsST(Scene* pScene, std::chrono::microseconds updateTime)
	: SceneThread(pScene, updateTime)
{
}

void PhysicsST::Inner(float delta)
{
	(delta);
	mpScene->GetPxScene()->lockWrite();
	mpScene->GetPxScene()->simulate(delta);
	mpScene->GetPxScene()->fetchResults(true);
	mpScene->GetPxScene()->unlockWrite();
}
