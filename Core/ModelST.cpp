#include "pch.h"
#include "ModelST.h"

using namespace Ion;

Core::ModelST::ModelST(Core::Scene* pScene, std::chrono::milliseconds updateTime)
	: Core::SceneThread(pScene, updateTime)
{
}

void Core::ModelST::Inner(float delta)
{
	if (!mpScene->TryLockExclusiveObjects())
		return;
	if (mpScene->TryLockSharedModelCs())
	{
		//for (auto& object : mpScene->GetObjects())
		//	object.ModelCUpdate(delta);
		mpScene->ModelCUpdate(delta);
		mpScene->UnlockSharedModelCs();
	}

	if (mpScene->TryLockExclusiveModelCs())
	{
		//for (auto& object : mpScene->GetObjects())
		//	object.ModelCSwitch();
		mpScene->ModelCSwitch();
		mpScene->UnlockExclusiveModelCs();
	}
	mpScene->UnlockExclusiveObjects();
}
