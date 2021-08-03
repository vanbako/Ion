#include "pch.h"
#include "SceneThread.h"

using namespace Ion;

Core::SceneThread::SceneThread(Core::Scene* pScene, std::chrono::microseconds updateTime)
	: mpScene{ pScene }
	, mUpdateTime{ updateTime }
	, mThread{}
	, mStats{}
	, mStatsMutex{}
	, mStatCurrent{ 0 }
{
	mStats.reserve(Core::Scene::GetStatCount());
	if (mThread.get_id() == std::thread::id{})
		mThread = std::thread{ &Loop, this };
}

Core::SceneThread::~SceneThread()
{
	if (mThread.joinable())
		mThread.join();
}

Core::Scene* Core::SceneThread::GetScene()
{
	return mpScene;
}

void Core::SceneThread::setUpdateTime(std::chrono::microseconds updateTime)
{
	mUpdateTime.store(updateTime);
}

const std::chrono::microseconds Core::SceneThread::GetUpdateTime() const
{
	return mUpdateTime.load();
}

void Core::SceneThread::Loop(Core::SceneThread* pSceneThread)
{
	std::srand(unsigned int(std::time(nullptr)));
	Core::Scene* pScene{ pSceneThread->GetScene() };
	std::chrono::steady_clock::time_point
		start{ std::chrono::steady_clock::now() },
		end{};
	std::chrono::microseconds sleep{};
	float delta{ 0.f };
	while (!pScene->GetIsEnd())
	{
		sleep = std::chrono::duration_cast<std::chrono::microseconds>(start - std::chrono::steady_clock::now()) + pSceneThread->mUpdateTime.load();
		std::this_thread::sleep_for(sleep);
		end = std::chrono::steady_clock::now();
		delta = float(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) / 1000000.f;
		start = end;
		if (pScene->GetIsActive())
			pSceneThread->Inner(delta);
	}
}
