#include "pch.h"
#include "Scene.h"
#include "ModelST.h"
#include "ControllerST.h"
#include "ViewST.h"
#include "PhysicsST.h"
#include "Application.h"
#include <extensions/PxExtensionsAPI.h>

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
	, mpModelST{ new ModelST{ this, (std::chrono::microseconds)5000 } }
	, mpControllerST{ new ControllerST{ this, (std::chrono::microseconds)6000 } }
	, mpViewST{ new ViewST{ this, (std::chrono::microseconds)15000 } }
	, mpPhysicsST{ new PhysicsST{ this, (std::chrono::microseconds)5000 } } // 8333 (120/s)
	, mpCanvases{}
	, mpPxScene{ nullptr }
	, mMutex{}
	, mConditionVar{}
{
	physx::PxSceneDesc sceneDesc{ mpApplication->GetToleranceScale() };
	sceneDesc.gravity = physx::PxVec3{ 0.0f, -9.81f, 0.0f };
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	sceneDesc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(1);
	mpPxScene = mpApplication->GetPhysics()->createScene(sceneDesc);
}

Scene::~Scene()
{
	delete mpModelST;
	delete mpControllerST;
	delete mpViewST;
	delete mpPhysicsST;
	mpPxScene->release();
	for (Canvas* pCanvas : mpCanvases)
	{
		std::lock_guard<std::mutex> lk(mMutex);
		pCanvas->SetThreadAction(ThreadAction::Close);
	}
	mConditionVar.notify_all();
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

const bool Scene::GetIsEnd() const
{
	return mIsEnd.load();
}

std::list<Object>& Scene::GetObjects()
{
	return mObjects;
}

Application* Scene::GetApplication()
{
	return mpApplication;
}

ControllerST* Scene::GetControllerST()
{
	return mpControllerST;
}

physx::PxScene* Scene::GetPxScene()
{
	return mpPxScene;
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

void Scene::AddCanvas(Canvas* pCanvas)
{
	mpCanvases.emplace(pCanvas);
	pCanvas->RunThread(&mConditionVar, &mMutex);
}

void Scene::Render()
{
	for (Canvas* pCanvas : mpCanvases)
	{
		std::lock_guard<std::mutex> lk(mMutex);
		pCanvas->SetThreadAction(ThreadAction::Render);
	}
	mConditionVar.notify_all();
}
