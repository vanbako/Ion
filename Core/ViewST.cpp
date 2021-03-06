#include "pch.h"
#include "ViewST.h"

using namespace Ion;

Core::ViewST::ViewST(Core::Scene* pScene, std::chrono::milliseconds updateTime)
	: Core::SceneThread(pScene, updateTime)
{
}

void Core::ViewST::Inner(float delta)
{
	if (!mpScene->TryLockSharedObjects())
		return;
	if (mpScene->TryLockSharedViewCs())
	{
		mpScene->ViewCUpdate(delta);
		mpScene->Render();
		mpScene->UnlockSharedViewCs();
	}
	mpScene->UnlockSharedObjects();
}
