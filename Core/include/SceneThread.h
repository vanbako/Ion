#pragma once
#include "ModelC.h"
#include "ControllerC.h"
#include "ViewC.h"
#include "Scene.h"

namespace Ion
{
	namespace Core
	{
		class Application;

		class SceneThread
		{
		public:
			explicit SceneThread(Core::Scene* pScene, std::chrono::milliseconds updateTime);
			virtual ~SceneThread();
			SceneThread(const SceneThread& other) = delete;
			SceneThread(SceneThread&& other) noexcept = delete;
			SceneThread& operator=(const SceneThread& other) = delete;
			SceneThread& operator=(SceneThread&& other) noexcept = delete;

			Core::Scene* GetScene();
			void setUpdateTime(std::chrono::milliseconds updateTime);
			const std::chrono::milliseconds GetUpdateTime() const;
#ifdef ION_STATS
			std::chrono::microseconds* GetStats();
			std::size_t* GetStatSeqs();
			std::size_t GetStatCount();
			std::size_t GetStatCurrent();
			void Done();
			bool TryLockStats();
			void UnlockStats();
#endif
		protected:
			static void Loop(Core::SceneThread* pSceneThread);
			virtual void Inner(float delta) = 0;

			Core::Scene* mpScene;
			std::atomic<std::chrono::milliseconds> mUpdateTime;
			std::thread mThread;
#ifdef ION_STATS
			static const std::chrono::milliseconds mStatsMutexDuration;
			static const std::chrono::milliseconds mStatsTime;
			static const std::size_t mStatMax{ 256 };

			std::timed_mutex mStatsMutex;
			std::chrono::microseconds mStats[mStatMax];
			std::size_t mStatSeqs[mStatMax];
			std::size_t mStatCount;
			std::size_t mStatCurrent;
			std::size_t mStatSeq;
#endif
		};
	}
}
