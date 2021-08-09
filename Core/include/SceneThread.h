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
			explicit SceneThread(Core::Scene* pScene, std::chrono::microseconds updateTime);
			virtual ~SceneThread();
			SceneThread(const SceneThread& other) = delete;
			SceneThread(SceneThread&& other) noexcept = delete;
			SceneThread& operator=(const SceneThread& other) = delete;
			SceneThread& operator=(SceneThread&& other) noexcept = delete;

			Core::Scene* GetScene();
			void setUpdateTime(std::chrono::microseconds updateTime);
			const std::chrono::microseconds GetUpdateTime() const;
		protected:
			static void Loop(Core::SceneThread* pSceneThread);

			virtual void Inner(float delta) = 0;

			Core::Scene* mpScene;
			std::atomic<std::chrono::microseconds> mUpdateTime;
			std::thread mThread;
#ifdef ION_STATS
			std::timed_mutex mStatsMutex;
			std::vector<std::chrono::microseconds> mStats;
			std::size_t mStatCurrent;
#endif
		};
	}
}
