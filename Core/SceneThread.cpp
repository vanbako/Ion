#include "../Core/pch.h"
#include "../Core/SceneThread.h"

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
	std::chrono::duration<float> sleep{};
	float delta{ 0.f };
	while (!pScene->GetIsEnd())
	{
		sleep = std::chrono::duration<float>(start + pSceneThread->mUpdateTime.load() - std::chrono::steady_clock::now());
		std::this_thread::sleep_for(sleep);
		end = std::chrono::steady_clock::now();
		delta = std::chrono::duration<float>(end - start).count();
		start = end;
		if (pScene->GetIsActive())
			pSceneThread->Inner(delta);
	}
}
