#include "../Core/pch.h"
#include "../Core/Scene.h"
#include "../Core/SceneThread.h"

using namespace Ion::Core;

std::chrono::microseconds Scene::mObjectsMutexDuration{ 1000 };
std::size_t Scene::mStatCount{ 300 }; // in seconds

std::size_t Scene::GetStatCount()
{
	return mStatCount;
}

Scene::Scene(Application* pApplication)
	: mpApplication{ pApplication }
	, mIsActive{ false }
	, mIsEnd{ false }
	, mObjectsMutex{}
	, mObjects{}
	, mpSceneThreadModelC{ new SceneThread<ModelC>{ this } }
	, mpSceneThreadControllerC{ new SceneThread<ControllerC>{ this} }
	, mpSceneThreadViewC{ new SceneThread<ViewC>{ this } }
	, mpCanvases{}
{
}

Scene::~Scene()
{
	delete mpSceneThreadModelC;
	delete mpSceneThreadControllerC;
	delete mpSceneThreadViewC;
}

void Scene::SetIsActive(bool isActive)
{
	mIsActive.store(isActive);
}

const bool Scene::GetIsActive() const
{
	return mIsActive.load();
}

void Scene::SetIsEnd(bool isEnd)
{
	mIsEnd.store(isEnd);
}

const bool Ion::Core::Scene::GetIsEnd() const
{
	return mIsEnd.load();
}

std::list<Object>& Ion::Core::Scene::GetObjects()
{
	return mObjects;
}

Application* Scene::GetApplication()
{
	return mpApplication;
}

void Scene::Initialize()
{
	if (!TryLockExclusiveObjects())
		return;
	for (auto& object : mObjects)
	{
		object.ModelCInitialize();
		object.ControllerCInitialize();
		object.ViewCInitialize();
	}
	UnlockExclusiveObjects();
}

Object* Scene::AddObject(bool isActive)
{
	if (!TryLockExclusiveObjects())
		return nullptr;
	mObjects.emplace_back(isActive, this);
	Core::Object* pObject{ &mObjects.back() };
	UnlockExclusiveObjects();
	return pObject;
}

bool Scene::TryLockSharedObjects()
{
	return mObjectsMutex.try_lock_shared_for(mObjectsMutexDuration);
}

void Scene::UnlockSharedObjects()
{
	mObjectsMutex.unlock_shared();
}

bool Scene::TryLockExclusiveObjects()
{
	return mObjectsMutex.try_lock_for(mObjectsMutexDuration);
}

void Scene::UnlockExclusiveObjects()
{
	mObjectsMutex.unlock();
}

void Ion::Core::Scene::AddCanvas(Canvas* pCanvas)
{
	mpCanvases.emplace(pCanvas);
}

void Ion::Core::Scene::Render()
{
	for (Canvas* pCanvas : mpCanvases)
		pCanvas->Render();
}
