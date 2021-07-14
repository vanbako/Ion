#include "pch.h"
#include "SceneThread.h"

using namespace Ion::Core;

SceneThread::SceneThread(Scene* pScene, std::chrono::microseconds updateTime)
	: mpScene{ pScene }
	, mUpdateTime{ updateTime }
	, mThread{}
	, mStats{}
	, mStatsMutex{}
	, mStatCurrent{ 0 }
{
	mStats.reserve(Scene::GetStatCount());
	if (mThread.get_id() == std::thread::id{})
		mThread = std::thread{ &Loop, this };
}

SceneThread::~SceneThread()
{
	if (mThread.joinable())
		mThread.join();
}

Scene* SceneThread::GetScene()
{
	return mpScene;
}

void SceneThread::setUpdateTime(std::chrono::microseconds updateTime)
{
	mUpdateTime.store(updateTime);
}

const std::chrono::microseconds Ion::Core::SceneThread::GetUpdateTime() const
{
	return mUpdateTime.load();
}

void SceneThread::Loop(SceneThread* pSceneThread)
{
	Scene* pScene{ pSceneThread->GetScene() };
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
