#include "../Core/pch.h"
#include "../Core/ViewST.h"

using namespace Ion::Core;

ViewST::ViewST(Scene* pScene)
	: SceneThread(pScene)
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
