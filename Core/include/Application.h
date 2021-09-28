#pragma once
#include "Scene.h"
#include "Material3D.h"
#include "Material2D.h"
#include "Model.h"
#include "Texture.h"
#include "Window.h"
#include "PxIonAllocatorCallback.h"
#include "PxIonErrorCallback.h"
#include "ServiceLocator.h"

namespace Ion
{
	namespace Core
	{
		class Application final
		{
		public:
			explicit Application();
			~Application();
			Application(const Application& other) = delete;
			Application(Application&& other) noexcept = delete;
			Application& operator=(const Application& other) = delete;
			Application& operator=(Application&& other) noexcept = delete;

			void ThrowIfFailed(HRESULT hr);
			bool Initialize();
			void Run();
			const bool SetIsActive(bool isActive);
			const bool GetIsActive() const;
			bool TryLockSharedKeyboard();
			void UnlockSharedKeyboard();
			PBYTE GetKeyboard();

			Core::Scene* AddScene(const std::string& name);
			Core::Scene* GetScene(size_t num);
			Core::Window* AddWindow(const std::wstring& title, RECT rectangle = RECT{ 0, 0, 1280, 720 });
			LRESULT WindowsProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
			const Microsoft::WRL::ComPtr<IDXGIFactory5>& GetDxgiFactory();
			const Microsoft::WRL::ComPtr<ID3D12Device5>& GetDevice();
			const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& GetCommandQueue();
			physx::PxPhysics* GetPxPhysics();
			physx::PxCooking* GetPxCooking();
			const physx::PxTolerancesScale& GetToleranceScale();
			Core::ServiceLocator& GetServiceLocator();

			Core::Material3D* AddMaterial3D(const std::string& name);
			Core::Material2D* AddMaterial2D(const std::string& name);
			Core::Model* AddModel(const std::string& fileName, const std::string& fileExtension, Winding winding, CoordSystem coordSystem);
			Core::Texture* AddTexture(const std::string& name);
		private:
			static const std::chrono::milliseconds
				mRunSleep,
				mKeyboardMutexDuration,
				mTextureMutexDuration,
				mModelMutexDuration,
				mMaterialMutexDuration;

			bool
				mIsInitialized,
				mIsActive;
			BYTE mKeyboard[256];
			std::shared_timed_mutex
				mKeyboardMutex,
				mMaterialMutex,
				mModelMutex,
				mTextureMutex;
			std::list<Core::Scene> mScenes;
			std::list<Core::Window> mWindows;
			std::map<std::string, Core::Material3D> mMaterials3D;
			std::map<std::string, Core::Material2D> mMaterials2D;
			std::map<std::string, Core::Model> mModels;
			std::map<std::string, Core::Texture> mTextures;
			Microsoft::WRL::ComPtr<IDXGIFactory5> mpDxgiFactory;
			Microsoft::WRL::ComPtr<ID3D12Device5> mpD3d12Device;
			Microsoft::WRL::ComPtr<IDXGIDevice1> mpDxgiDevice;
			Microsoft::WRL::ComPtr<ID3D12CommandQueue> mpCommandQueue;
			physx::PxPhysics* mpPhysics;
			physx::PxTolerancesScale mPxToleranceScale;
			physx::PxIonAllocatorCallback mIonAllocatorCallback;
			physx::PxIonErrorCallback mIonErrorCallback;
			physx::PxCooking* mpCooking;
			Core::ServiceLocator mServiceLocator;

			void KeyboardState();
		};
	}
}
