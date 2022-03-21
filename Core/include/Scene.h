#pragma once
#include "Object.h"
#include "Cube.h"
#include "ModelC.h"
#include "ControllerC.h"
#include "ViewC.h"
#include "Canvas.h"
#include "Window.h"
#include "ModelCCube.h"
#include "ControllerCCube.h"

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
			explicit Scene(const std::string& name, Core::Application* pApplication);
			~Scene();
			Scene(const Scene& other) = delete;
			Scene(Scene&& other) noexcept = delete;
			Scene& operator=(const Scene& other) = delete;
			Scene& operator=(Scene&& other) noexcept = delete;

			void SetIsActive(bool isActive);
			const bool GetIsActive() const;
			void SetIsEnd(bool isEnd);
			const bool GetIsEnd() const;
			const std::string& GetName();
			std::list<Core::Object>& GetObjects();
			Core::Application* GetApplication();
			Core::ControllerST* GetControllerST();
			physx::PxScene* GetPxScene();
			physx::PxControllerManager* GetPxControllerManager();
			std::unordered_map<std::string, Core::SceneThread*>& GetSceneThreads();

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
			void ControllerCUpdate(float delta);
			void ModelCUpdate(float delta);
			void ModelCSwitch();
			void ViewCUpdate(float delta);
			void Render();
			void AddModelC(Core::ModelC* pModelC);
			void MoveModelC(Core::ModelC* pModelC, Core::ModelCCube* pCurrCube);
			void AddModelCToCube(Core::ModelC* pModelC);
			void AddControllerC(Core::ControllerC* pControllerC);
			void MoveControllerC(Core::ControllerC* pControllerC, Core::ControllerCCube* pCurrCube);
			void AddControllerCToCube(Core::ControllerC* pControllerC);
		private:
			static Core::Vector<long long> mModelCubeSize;
			static Core::Vector<long long> mControllerCubeSize;
			static std::chrono::milliseconds mObjectsMutexDuration;
			static std::chrono::milliseconds
				mModelTime,
				mControllerTime,
				mViewTime,
				mPhysicsTime;
			const std::string mName;
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
			std::unordered_map<std::string, Core::SceneThread*> mpSceneThreads;
			Core::ModelST* mpModelST;
			Core::ControllerST* mpControllerST;
			Core::ViewST* mpViewST;
			Core::PhysicsST* mpPhysicsST;
#ifdef ION_STATS
			static std::chrono::milliseconds mStatsTime;
			Core::StatsST* mpStatsST;
#endif
			std::unordered_map<Core::Canvas*, std::pair<std::mutex, std::condition_variable>> mpCanvases;
			physx::PxScene* mpPxScene;
			physx::PxControllerManager* mpPxControllerManager;
			std::multimap<long long, Core::ModelCCube> mModelCCubes;
			std::multimap<long long, Core::ControllerCCube> mControllerCCubes;

			Core::Vector<long long> GetModelCubePos(Core::ModelC* pModelC);
			std::multimap<long long, Core::ModelCCube>::iterator GetModelCubeIterator(Core::Vector<long long>& cubePos);
			Core::Vector<long long> GetControllerCubePos(Core::ControllerC* pControllerC);
			std::multimap<long long, Core::ControllerCCube>::iterator GetControllerCubeIterator(Core::Vector<long long>& cubePos);
		};
	}
}
