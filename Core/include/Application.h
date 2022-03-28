#pragma once
#include "Scene.h"
#include "ResourceManager.h"
#include "Window.h"
#include "Canvas.h"
#include "PxIonAllocatorCallback.h"
#include "PxIonErrorCallback.h"
#include "ServiceLocator.h"
#include "CursorMode.h"

namespace Ion
{
	namespace Core
	{
		LRESULT CALLBACK AppWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

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
			Core::CursorMode GetCursorMode() const;
			void SetCursorMode(Core::CursorMode cursorMode);
			bool TryLockSharedKeyboard();
			void UnlockSharedKeyboard();
			PBYTE GetKeyboard();
			POINT GetCursorPosition();

			Core::Scene* AddScene(const std::string& name);
			Core::Scene* GetScene(std::size_t num);
			Core::Window* AddWindow(const std::wstring& title, RECT rectangle = RECT{ 0, 0, 1280, 720 });
			Core::Canvas* AddCanvas(RECT rectangle);
			std::list<Core::Canvas>& GetCanvases();
			LRESULT WindowsProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
			const Microsoft::WRL::ComPtr<IDXGIFactory5>& GetDxgiFactory();
			const Microsoft::WRL::ComPtr<ID3D12Device5>& GetDevice();
			const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& GetCommandQueue();
			physx::PxPhysics* GetPxPhysics();
			physx::PxCooking* GetPxCooking();
			const physx::PxTolerancesScale& GetToleranceScale();
			Core::ServiceLocator& GetServiceLocator();
			ResourceManager* GetResourceManager();
		private:
			static const std::chrono::milliseconds
				mRunSleep,
				mKeyboardMutexDuration;

			bool
				mIsInitialized,
				mIsActive;
			CursorMode mCursorMode;
			std::atomic<bool>
				mHideCursor,
				mShowCursor;
			BYTE mKeyboard[256];
			std::shared_timed_mutex mKeyboardMutex;
			HCURSOR mCursor;
			std::atomic<POINT> mCursorSavePos;
			std::list<Core::Scene> mScenes;
			std::list<Core::Window> mWindows;
			// TODO: Add mutex for Canvas if you want to add/remove Canvases during gameplay
			std::list<Core::Canvas> mCanvases;
			Core::ResourceManager mResourceManager;
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
