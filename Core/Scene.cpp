#include "pch.h"
#include "Scene.h"
#include "ModelST.h"
#include "ControllerST.h"
#include "ViewST.h"
#include "PhysicsST.h"
#include "Application.h"
#include <extensions/PxExtensionsAPI.h>

using namespace Ion;

std::chrono::microseconds Core::Scene::mObjectsMutexDuration{ 1000 };
std::size_t Core::Scene::mStatCount{ 300 }; // in seconds

std::size_t Core::Scene::GetStatCount()
{
	return mStatCount;
}

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
	, mpModelST{ new Core::ModelST{ this, (std::chrono::microseconds)3000 } }
	, mpControllerST{ new Core::ControllerST{ this, (std::chrono::microseconds)3000 } }
	, mpViewST{ new Core::ViewST{ this, (std::chrono::microseconds)3000 } }
	, mpPhysicsST{ new Core::PhysicsST{ this, (std::chrono::microseconds)3000 } }
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
	mpPxScene->release();
	for (Core::Canvas* pCanvas : mpCanvases)
	{
		std::lock_guard<std::mutex> lk(mMutex);
		pCanvas->SetThreadAction(Core::ThreadAction::Close);
	}
	mConditionVar.notify_all();
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
	mpCanvases.emplace(pCanvas);
	pCanvas->RunThread(&mConditionVar, &mMutex);
}

void Core::Scene::Render()
{
#ifdef _DEBUG
	if (!mIsInitialized)
	{
		mpApplication->GetServiceLocator().GetLogger()->Message(this, Core::MsgType::Fatal, "Scene.Render() while mIsInitialized == false");
		return;
	}
#endif
	for (Core::Canvas* pCanvas : mpCanvases)
	{
		std::lock_guard<std::mutex> lk(mMutex);
		pCanvas->SetThreadAction(Core::ThreadAction::Render);
	}
	mConditionVar.notify_all();
}
