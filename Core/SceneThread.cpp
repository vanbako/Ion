#include "pch.h"
#include "SceneThread.h"
#include <timeapi.h>

using namespace Ion;

#ifdef ION_STATS
const std::chrono::milliseconds Core::SceneThread::mStatsMutexDuration{ 1 };
const std::chrono::milliseconds Core::SceneThread::mStatsTime{ 32 };
#endif

Core::SceneThread::SceneThread(Core::Scene* pScene, std::chrono::milliseconds updateTime)
	: mpScene{ pScene }
	, mUpdateTime{ updateTime }
	, mThread{}
#ifdef ION_STATS
	, mStats{}
	, mStatSeqs{}
	, mStatsMutex{}
	, mStatCount{ 0 }
	, mStatCurrent{ 0 }
	, mStatSeq{ 0 }
#endif
{
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

void Core::SceneThread::setUpdateTime(std::chrono::milliseconds updateTime)
{
	mUpdateTime.store(updateTime);
}

const std::chrono::milliseconds Core::SceneThread::GetUpdateTime() const
{
	return mUpdateTime.load();
}

#ifdef ION_STATS
std::chrono::microseconds* Core::SceneThread::GetStats()
{
	return mStats;
}

std::size_t* Ion::Core::SceneThread::GetStatSeqs()
{
	return mStatSeqs;
}

std::size_t Core::SceneThread::GetStatCount()
{
	return mStatCount;
}

std::size_t Core::SceneThread::GetStatCurrent()
{
	return mStatCurrent;
}

void Core::SceneThread::Done()
{
	mStatCount = 0;
}

bool Core::SceneThread::TryLockStats()
{
	return mStatsMutex.try_lock_for(mStatsMutexDuration);
}

void Ion::Core::SceneThread::UnlockStats()
{
	mStatsMutex.unlock();
}
#endif

void Core::SceneThread::Loop(Core::SceneThread* pSceneThread)
{
	timeBeginPeriod(1);
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
		std::chrono::steady_clock::time_point beforeSleep{ std::chrono::steady_clock::now() };
		if (sleep.count() > 0)
			if (sleep <= pSceneThread->mUpdateTime.load())
				std::this_thread::sleep_for(sleep);
			else
				std::this_thread::sleep_for(pSceneThread->mUpdateTime.load());
		std::chrono::steady_clock::time_point afterSleep{ std::chrono::steady_clock::now() };
		end = std::chrono::steady_clock::now();
		delta = float(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) / 1000000.f;
		start = end;
		if (pScene->GetIsActive())
			pSceneThread->Inner(delta);
#ifdef ION_STATS
		if (pSceneThread->TryLockStats())
		{
			++pSceneThread->mStatSeq;
			++pSceneThread->mStatCurrent;
			++pSceneThread->mStatCount;
			if (pSceneThread->mStatCount >= mStatMax)
				pSceneThread->mStatCount = mStatMax;
			if (pSceneThread->mStatCurrent >= mStatMax)
				pSceneThread->mStatCurrent = 0;
			pSceneThread->mStats[pSceneThread->mStatCurrent] = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start);
			pSceneThread->UnlockStats();
			pSceneThread->mStatSeqs[pSceneThread->mStatCurrent] = pSceneThread->mStatSeq;
		}
#endif
	}
	timeEndPeriod(1);
}
