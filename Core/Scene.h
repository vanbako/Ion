#pragma once
#include "../Core/Object.h"
#include "../Core/ModelC.h"
#include "../Core/ControllerC.h"
#include "../Core/ViewC.h"
#include "../Core/Canvas.h"
#include "../Core/Window.h"

namespace Ion
{
	namespace Core
	{
		class Application;
		template<class T>
		class SceneThread;

		class Scene final
		{
		public:
			static std::size_t GetStatCount();

			explicit Scene(Application* pApplication);
			~Scene();
			Scene(const Scene& other) = delete;
			Scene(Scene&& other) noexcept = delete;
			Scene& operator=(const Scene& other) = delete;
			Scene& operator=(Scene&& other) noexcept = delete;

			void SetIsActive(bool isActive);
			const bool GetIsActive() const;
			const bool GetIsEnd() const;
			std::list<Object>& GetObjects();
			Application* GetApplication();

			void Initialize();
			Object* AddObject(bool isActive);
			bool TryLockSharedObjects();
			void UnlockSharedObjects();
			bool TryLockExclusiveObjects();
			void UnlockExclusiveObjects();
			void AddCanvas(Canvas* pCanvas);
			void Render();
		private:
			static std::chrono::microseconds mObjectsMutexDuration;
			static std::size_t mStatCount;

			Application* mpApplication;
			std::atomic<bool>
				mIsActive,
				mIsEnd;
			std::shared_timed_mutex mObjectsMutex;
			std::list<Object> mObjects;
			SceneThread<ModelC>* mpSceneThreadModelC;
			SceneThread<ControllerC>* mpSceneThreadControllerC;
			SceneThread<ViewC>* mpSceneThreadViewC;
			std::set<Canvas*> mpCanvases;
		};
	}
}
