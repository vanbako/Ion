#include "pch.h"
#include "Scene.h"
#include "ModelST.h"
#include "ControllerST.h"
#include "ViewST.h"
#include "PhysicsST.h"
#ifdef ION_STATS
#include "StatsST.h"
#endif
#include "Application.h"
#include <extensions/PxExtensionsAPI.h>

using namespace Ion;

std::chrono::microseconds Core::Scene::mObjectsMutexDuration{ 1000 };
std::chrono::microseconds Core::Scene::mModelTime{ 3000 };
std::chrono::microseconds Core::Scene::mControllerTime{ 3000 };
std::chrono::microseconds Core::Scene::mViewTime{ 6500 };
std::chrono::microseconds Core::Scene::mPhysicsTime{ 3000 };

#ifdef ION_STATS
std::chrono::microseconds Core::Scene::mStatsTime{ 32000 };
#endif

Core::Scene::Scene(Core::Application* pApplication)
	: mpApplication{ pApplication }
	, mIsInitialized{ false }
	, mIsActive{ false }
	, mIsEnd{ false }
	, mObjectsMutex{}
	, mControllerCMutex{}
	, mModelCMutex{}
	, mViewCMutex{}
	, mObjects{}
	, mpModelST{ new Core::ModelST{ this, mModelTime } }
	, mpControllerST{ new Core::ControllerST{ this, mControllerTime } }
	, mpViewST{ new Core::ViewST{ this, mViewTime } }
	, mpPhysicsST{ new Core::PhysicsST{ this, mPhysicsTime } }
#ifdef ION_STATS
	, mpStatsST{ nullptr }
#endif
	, mpCanvases{}
	, mpPxScene{ nullptr }
{
	mpSceneThreads = {
		{ "Model", mpModelST },
		{ "Controller", mpControllerST },
		{ "View", mpViewST },
		{ "Physics", mpPhysicsST }
	};
#ifdef ION_STATS
	mpStatsST = new Core::StatsST{ this, mStatsTime };
#endif
	physx::PxSceneDesc sceneDesc{ mpApplication->GetToleranceScale() };
	sceneDesc.gravity = physx::PxVec3{ 0.0f, -9.81f, 0.0f };
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	sceneDesc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(1);
	mpPxScene = mpApplication->GetPhysics()->createScene(sceneDesc);
//#ifdef _DEBUG
//	mpPxScene->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, 1.0f);
//	mpPxScene->setVisualizationParameter(physx::PxVisualizationParameter::eACTOR_AXES, 2.0f);
//	mpPxScene->setVisualizationParameter(physx::PxVisualizationParameter::eBODY_LIN_VELOCITY, 2.0f);
//#endif
}

Core::Scene::~Scene()
{
	delete mpModelST;
	delete mpControllerST;
	delete mpViewST;
	delete mpPhysicsST;
#ifdef ION_STATS
	delete mpStatsST;
#endif
	mpPxScene->release();
	for (auto& pair : mpCanvases)
	{
		std::lock_guard<std::mutex> lk(pair.second.first);
		pair.first->SetThreadAction(Core::ThreadAction::Close);
		pair.second.second.notify_one();
	}
	// TODO: wait for Canvases to finish
}

void Core::Scene::SetIsActive(bool isActive)
{
	mIsActive.store(isActive);
}

const bool Core::Scene::GetIsActive() const
{
	return mIsActive.load();
}

void Core::Scene::SetIsEnd(bool isEnd)
{
	mIsEnd.store(isEnd);
}

const bool Core::Scene::GetIsEnd() const
{
	return mIsEnd.load();
}

std::list<Core::Object>& Core::Scene::GetObjects()
{
	return mObjects;
}

Core::Application* Core::Scene::GetApplication()
{
	return mpApplication;
}

Core::ControllerST* Core::Scene::GetControllerST()
{
	return mpControllerST;
}

physx::PxScene* Core::Scene::GetPxScene()
{
	return mpPxScene;
}

std::map<const std::string, Core::SceneThread*>& Core::Scene::GetSceneThreads()
{
	return mpSceneThreads;
}

void Core::Scene::Initialize()
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
	mIsInitialized = true;
}

Core::Object* Core::Scene::AddObject(bool isActive)
{
	if (!TryLockExclusiveObjects())
		return nullptr;
	Core::Object* pObject{ &mObjects.emplace_back(isActive, this) };
	UnlockExclusiveObjects();
	return pObject;
}

bool Core::Scene::TryLockSharedObjects()
{
	return mObjectsMutex.try_lock_shared_for(mObjectsMutexDuration);
}

void Core::Scene::UnlockSharedObjects()
{
	mObjectsMutex.unlock_shared();
}

bool Core::Scene::TryLockExclusiveObjects()
{
	return mObjectsMutex.try_lock_for(mObjectsMutexDuration);
}

void Core::Scene::UnlockExclusiveObjects()
{
	mObjectsMutex.unlock();
}

bool Core::Scene::TryLockSharedControllerCs()
{
	return mControllerCMutex.try_lock_shared_for(mObjectsMutexDuration);
}

void Core::Scene::UnlockSharedControllerCs()
{
	mControllerCMutex.unlock_shared();
}

bool Core::Scene::TryLockExclusiveControllerCs()
{
	return mControllerCMutex.try_lock_for(mObjectsMutexDuration);
}

void Core::Scene::UnlockExclusiveControllerCs()
{
	mControllerCMutex.unlock();
}

bool Core::Scene::TryLockSharedModelCs()
{
	return mModelCMutex.try_lock_shared_for(mObjectsMutexDuration);
}

void Core::Scene::UnlockSharedModelCs()
{
	mModelCMutex.unlock_shared();
}

bool Core::Scene::TryLockExclusiveModelCs()
{
	return mModelCMutex.try_lock_for(mObjectsMutexDuration);
}

void Core::Scene::UnlockExclusiveModelCs()
{
	mModelCMutex.unlock();
}

bool Core::Scene::TryLockSharedViewCs()
{
	return mViewCMutex.try_lock_shared_for(mObjectsMutexDuration);
}

void Core::Scene::UnlockSharedViewCs()
{
	mViewCMutex.unlock_shared();
}

bool Core::Scene::TryLockExclusiveViewCs()
{
	return mViewCMutex.try_lock_for(mObjectsMutexDuration);
}

void Core::Scene::UnlockExclusiveViewCs()
{
	mViewCMutex.unlock();
}

void Core::Scene::AddCanvas(Core::Canvas* pCanvas)
{
	auto pair{ mpCanvases.try_emplace(pCanvas) };
	// If pCanvas was not yet in the mpCanvases map
	if (pair.second)
		pCanvas->RunThread(&(*pair.first).second.second, &(*pair.first).second.first);
}

void Core::Scene::Render()
{
	if (!mIsInitialized)
	{
#ifdef ION_LOGGER
		mpApplication->GetServiceLocator().GetLogger()->Message(typeid(this).name(), Core::MsgType::Fatal, "Scene.Render() while mIsInitialized == false");
#endif
		return;
	}
	for (auto& pair : mpCanvases)
	{
		pair.second.first.lock();
		pair.first->SetThreadAction(Core::ThreadAction::Render);
		pair.second.second.notify_one();
		pair.second.first.unlock();
	}
}
