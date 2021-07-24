#include "pch.h"
#include "ViewST.h"

using namespace Ion;

Core::ViewST::ViewST(Core::Scene* pScene, std::chrono::microseconds updateTime)
	: Core::SceneThread(pScene, updateTime)
{
}

void Core::ViewST::Inner(float delta)
{
	if (!mpScene->TryLockSharedObjects())
		return;
	for (auto& object : mpScene->GetObjects())
		object.ViewCUpdate(delta);
	mpScene->Render();
	mpScene->UnlockSharedObjects();
}
