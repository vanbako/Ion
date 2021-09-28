#include "pch.h"
#include "StatsST.h"
#include "Scene.h"

#ifdef ION_STATS
using namespace Ion;

Core::StatsST::StatsST(Core::Scene* pScene, std::chrono::milliseconds updateTime)
	: Core::SceneThread(pScene, updateTime)
{
	auto& pSceneThreads{ mpScene->GetSceneThreads() };
	for (auto& pSceneThread : pSceneThreads)
		mpSceneThreads.emplace(pSceneThread.first, std::pair<Core::SceneThread*, std::ofstream>{ pSceneThread.second, std::ofstream{ mpScene->GetName() + "_" + pSceneThread.first + ".log", std::ios::out} });
}

Core::StatsST::~StatsST()
{
	for (auto& pSceneThread : mpSceneThreads)
		pSceneThread.second.second.close();
}

void Core::StatsST::Inner(float delta)
{
	(delta);
	for (auto& pSceneThread : mpSceneThreads)
		if (pSceneThread.second.first->TryLockStats())
			Stats(pSceneThread.second);
}

void Core::StatsST::Stats(std::pair<Core::SceneThread*, std::ofstream>& pSceneThread)
{
	std::memcpy(mStats, pSceneThread.first->GetStats(), mStatMax * sizeof(std::chrono::milliseconds));
	std::memcpy(mStatSeqs, pSceneThread.first->GetStatSeqs(), mStatMax * sizeof(size_t));
	std::size_t statCurrent{ pSceneThread.first->GetStatCurrent() };
	std::size_t statCount{ pSceneThread.first->GetStatCount() };
	pSceneThread.first->Done();
	pSceneThread.first->UnlockStats();
	if (statCount == 0)
		return;
	std::size_t cur{ 0 };
	if (statCount > statCurrent)
		cur = mStatMax - statCount + statCurrent;
	else
		cur = statCurrent - statCount;
	for (std::size_t i{ 0 }; i < statCount; ++i)
	{
		pSceneThread.second << mStatSeqs[cur] << " " << mStats[cur] << std::endl;
		++cur;
		if (cur >= mStatMax)
			cur = 0;
	}
}
#endif
