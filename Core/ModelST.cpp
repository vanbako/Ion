#include "pch.h"
#include "ModelST.h"

using namespace Ion::Core;

ModelST::ModelST(Scene* pScene, std::chrono::microseconds updateTime)
	: SceneThread(pScene, updateTime)
{
}

void ModelST::Inner(float delta)
{
	if (!mpScene->TryLockSharedObjects())
		return;
	for (auto& object : mpScene->GetObjects())
		object.ModelCUpdate(delta);
	mpScene->UnlockSharedObjects();
	if (!mpScene->TryLockExclusiveObjects())
		return;
	for (auto& object : mpScene->GetObjects())
		object.ModelCSwitch();
	mpScene->UnlockExclusiveObjects();
}
