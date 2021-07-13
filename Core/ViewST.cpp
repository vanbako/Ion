#include "pch.h"
#include "ViewST.h"

using namespace Ion::Core;

ViewST::ViewST(Scene* pScene, std::chrono::microseconds updateTime)
	: SceneThread(pScene, updateTime)
{
}

void ViewST::Inner(float delta)
{
	if (!mpScene->TryLockSharedObjects())
		return;
	for (auto& object : mpScene->GetObjects())
		object.ViewCUpdate(delta);
	mpScene->Render();
	mpScene->UnlockSharedObjects();
}
