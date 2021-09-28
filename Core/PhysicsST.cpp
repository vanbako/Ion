#include "pch.h"
#include "Application.h"
#include "PhysicsST.h"

using namespace Ion;

Core::PhysicsST::PhysicsST(Core::Scene* pScene, std::chrono::milliseconds updateTime)
	: Core::SceneThread(pScene, updateTime)
{
}

void Core::PhysicsST::Inner(float delta)
{
	mpScene->GetPxScene()->lockWrite();
	mpScene->GetPxScene()->simulate(delta);
	mpScene->GetPxScene()->fetchResults(true);
	mpScene->GetPxScene()->unlockWrite();
}
