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

		class Scene final
		{
		public:
#ifdef ION_STATS
			static std::size_t GetStatCount();
#endif

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
#ifdef ION_STATS
			static std::size_t mStatCount;
#endif
			Core::Application* mpApplication;
			std::atomic<bool>
				mIsInitialized,
				mIsActive,
				mIsEnd;
			std::shared_timed_mutex mObjectsMutex;
			std::shared_timed_mutex mControllerCMutex;
			std::shared_timed_mutex mModelCMutex;
			std::shared_timed_mutex mViewCMutex;
			std::list<Core::Object> mObjects;
			Core::ModelST* mpModelST;
			Core::ControllerST* mpControllerST;
			Core::ViewST* mpViewST;
			Core::PhysicsST* mpPhysicsST;
			std::map<Core::Canvas*, std::pair<std::mutex, std::condition_variable>> mpCanvases;
			physx::PxScene* mpPxScene;
		};
	}
}
