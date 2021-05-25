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

		template<class T>
		class SceneThread final
		{
		public:
			explicit SceneThread(Scene* pScene)
				: mpScene{ pScene }
				, mUpdateTime{ mUpdateTimeDefault }
				, mThread{}
				, mStats{}
				, mStatsMutex{}
				, mStatCurrent{ 0 }
			{
				mStats.reserve(Scene::GetStatCount());
				if (mThread.get_id() == std::thread::id{})
					mThread = std::thread{ &Loop, this };
			}
			~SceneThread()
			{
				if (mThread.joinable())
					mThread.join();
			}
			SceneThread(const SceneThread& other) = delete;
			SceneThread(SceneThread&& other) noexcept = delete;
			SceneThread& operator=(const SceneThread& other) = delete;
			SceneThread& operator=(SceneThread&& other) noexcept = delete;

			Scene* GetScene()
			{
				return mpScene;
			}
			void setUpdateTime(std::chrono::microseconds updateTime)
			{
				mUpdateTime.store(updateTime);
			}
			const std::chrono::microseconds GetUpdateTime() const
			{
				return mUpdateTime.load();
			}
		private:
			static inline std::chrono::microseconds mUpdateTimeDefault{ 5000 };

			Scene* mpScene;
			std::atomic<std::chrono::microseconds> mUpdateTime;
			std::thread mThread;
			std::timed_mutex mStatsMutex;
			std::vector<std::chrono::microseconds> mStats;
			std::size_t mStatCurrent;

			template<class T>
			static void Inner(T*, Scene* pScene, float delta) {};
			template<>
			static void Inner(ModelC*, Scene* pScene, float delta)
			{
				for (auto& object : pScene->GetObjects())
					object.ModelCUpdate(delta);
				pScene->UnlockSharedObjects();
				if (pScene->TryLockExclusiveObjects())
				{
					for (auto& object : pScene->GetObjects())
						object.ModelCSwitch();
					pScene->UnlockExclusiveObjects();
				}
			}
			template<>
			static void Inner(ControllerC*, Scene* pScene, float delta)
			{
				for (auto& object : pScene->GetObjects())
					object.ControllerCUpdate(delta);
				pScene->UnlockSharedObjects();
			}
			template<>
			static void Inner(ViewC*, Scene* pScene, float delta)
			{
				for (auto& object : pScene->GetObjects())
					object.ViewCUpdate(delta);
				pScene->Render();
				pScene->UnlockSharedObjects();
			}
			static void Loop(SceneThread* pSceneThread)
			{
				Scene* pScene{ pSceneThread->GetScene() };
				std::chrono::steady_clock::time_point
					start{ std::chrono::steady_clock::now() },
					end{};
				std::chrono::duration<float> sleep{};
				float delta{};
				while (!pScene->GetIsEnd())
				{
					sleep = std::chrono::duration<float>(start + pSceneThread->mUpdateTime.load() - std::chrono::steady_clock::now());
					std::this_thread::sleep_for(sleep);
					end = std::chrono::steady_clock::now();
					delta = std::chrono::duration<float>(end - start).count();
					start = end;
					if (pScene->GetIsActive() && pScene->TryLockSharedObjects())
						Inner((T*)nullptr, pScene, delta);
				}
			}
		};
	}
}
