#include "../Core/pch.h"
#include "../Core/Application.h"
#include "../Core/PhysicsST.h"

using namespace Ion::Core;

PhysicsST::PhysicsST(Scene* pScene, std::chrono::microseconds updateTime)
	: SceneThread(pScene, updateTime)
{
}

void PhysicsST::Inner(float delta)
{
	(delta);
	mpScene->GetPxScene()->lockWrite();
	// TODO: use delta after fixing the loop timing
	mpScene->GetPxScene()->simulate(1.f / 120.f);
	mpScene->GetPxScene()->fetchResults(true);
	mpScene->GetPxScene()->unlockWrite();
}
