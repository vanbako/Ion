#pragma once
#include "Object.h"
#include "ModelC.h"
#include "ControllerC.h"
#include "ViewC.h"
#include "Canvas.h"
#include "Window.h"

namespace Ion
{
	namespace Core
	{
		class Application;
		class ModelST;
		class ControllerST;
		class ViewST;
		class PhysicsST;
#ifdef ION_STATS
		class StatsST;
#endif
		class SceneThread;

		class Scene final
		{
		public:
			explicit Scene(Core::Application* pApplication);
			~Scene();
			Scene(const Scene& other) = delete;
			Scene(Scene&& other) noexcept = delete;
			Scene& operator=(const Scene& other) = delete;
			Scene& operator=(Scene&& other) noexcept = delete;

			void SetIsActive(bool isActive);
			const bool GetIsActive() const;
			void SetIsEnd(bool isEnd);
			const bool GetIsEnd() const;
			std::list<Core::Object>& GetObjects();
			Core::Application* GetApplication();
			Core::ControllerST* GetControllerST();
			physx::PxScene* GetPxScene();
			std::map<const std::string, Core::SceneThread*>& GetSceneThreads();

			void Initialize();
			Core::Object* AddObject(bool isActive);
			bool TryLockSharedObjects();
			void UnlockSharedObjects();
			bool TryLockExclusiveObjects();
			void UnlockExclusiveObjects();
			bool TryLockSharedControllerCs();
			void UnlockSharedControllerCs();
			bool TryLockExclusiveControllerCs();
			void UnlockExclusiveControllerCs();
			bool TryLockSharedModelCs();
			void UnlockSharedModelCs();
			bool TryLockExclusiveModelCs();
			void UnlockExclusiveModelCs();
			bool TryLockSharedViewCs();
			void UnlockSharedViewCs();
			bool TryLockExclusiveViewCs();
			void UnlockExclusiveViewCs();
			void AddCanvas(Core::Canvas* pCanvas);
			void Render();
		private:
			static std::chrono::microseconds mObjectsMutexDuration;
			static std::chrono::microseconds
				mModelTime,
				mControllerTime,
				mViewTime,
				mPhysicsTime;
			Core::Application* mpApplication;
			std::atomic<bool>
				mIsInitialized,
				mIsActive,
				mIsEnd;
			std::shared_timed_mutex
				mObjectsMutex,
				mControllerCMutex,
				mModelCMutex,
				mViewCMutex;
			std::list<Core::Object> mObjects;
			std::map<const std::string, Core::SceneThread*> mpSceneThreads;
			Core::ModelST* mpModelST;
			Core::ControllerST* mpControllerST;
			Core::ViewST* mpViewST;
			Core::PhysicsST* mpPhysicsST;
#ifdef ION_STATS
			static std::chrono::microseconds mStatsTime;
			Core::StatsST* mpStatsST;
#endif
			std::map<Core::Canvas*, std::pair<std::mutex, std::condition_variable>> mpCanvases;
			physx::PxScene* mpPxScene;
		};
	}
}
