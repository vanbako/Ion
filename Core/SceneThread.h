#pragma once
#include "../Core/ModelC.h"
#include "../Core/ControllerC.h"
#include "../Core/ViewC.h"
#include "../Core/Scene.h"

namespace Ion
{
	namespace Core
	{
		class Application;

		class SceneThread
		{
		public:
			explicit SceneThread(Scene* pScene, std::chrono::microseconds updateTime);
			virtual ~SceneThread();
			SceneThread(const SceneThread& other) = delete;
			SceneThread(SceneThread&& other) noexcept = delete;
			SceneThread& operator=(const SceneThread& other) = delete;
			SceneThread& operator=(SceneThread&& other) noexcept = delete;

			Scene* GetScene();
			void setUpdateTime(std::chrono::microseconds updateTime);
			const std::chrono::microseconds GetUpdateTime() const;
		protected:
			static void Loop(SceneThread* pSceneThread);

			virtual void Inner(float delta) = 0;

			Scene* mpScene;
			std::atomic<std::chrono::microseconds> mUpdateTime;
			std::thread mThread;
			std::timed_mutex mStatsMutex;
			std::vector<std::chrono::microseconds> mStats;
			std::size_t mStatCurrent;
		};
	}
}
