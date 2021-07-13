#pragma once
#include "Scene.h"
#include "Material3D.h"
#include "Material2D.h"
#include "Model.h"
#include "Texture.h"
#include "Window.h"
#include "PxIonAllocatorCallback.h"
#include "PxIonErrorCallback.h"

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

			bool Initialize();
			void Run();
			const bool SetIsActive(bool isActive);
			const bool GetIsActive() const;
			bool TryLockSharedKeyboard();
			void UnlockSharedKeyboard();
			PBYTE GetKeyboard();

			Scene* AddScene();
			Window* AddWindow(const std::wstring& title, RECT rectangle = RECT{ 0, 0, 1280, 720 });
			LRESULT WindowsProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
			const Microsoft::WRL::ComPtr<IDXGIFactory4>& GetDxgiFactory();
			const Microsoft::WRL::ComPtr<ID3D12Device>& GetDevice();
			const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& GetCommandQueue();
			const Microsoft::WRL::ComPtr<ID3D12CommandAllocator>& GetCommandAllocator();
			const Microsoft::WRL::ComPtr<ID2D1Factory3>& GetD2d1Factory();
			const Microsoft::WRL::ComPtr<IDXGIDevice>& GetDxgiDevice();
			const Microsoft::WRL::ComPtr<ID3D11On12Device>& GetD3D11On12Device();
			const Microsoft::WRL::ComPtr<ID2D1Device2>& GetD2d1Device();
			const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& GetD3d11DeviceContext();
			const Microsoft::WRL::ComPtr<ID2D1DeviceContext2>& GetD2d1DeviceContext();
			//const Microsoft::WRL::ComPtr<IDWriteFactory>& GetDWriteFactory();
			physx::PxPhysics* GetPhysics();
			const physx::PxTolerancesScale& GetToleranceScale();

			Material3D* AddMaterial3D(const std::string& name);
			Material2D* AddMaterial2D(const std::string& name);
			Model* AddModel(const std::string& name, Winding winding);
			Texture* AddTexture(const std::string& name);
		private:
			static const std::chrono::microseconds mRunSleep;
			static const std::chrono::microseconds mKeyboardMutexDuration;
			
			bool
				mIsInitialized,
				mIsActive;
			BYTE mKeyboard[256];
			std::shared_timed_mutex mKeyboardMutex;
			std::list<Scene> mScenes;
			std::list<Window> mWindows;
			std::map<std::string, Material3D> mMaterials3D;
			std::map<std::string, Material2D> mMaterials2D;
			std::map<std::string, Model> mModels;
			std::map<std::string, Texture> mTextures;
			Microsoft::WRL::ComPtr<IDXGIFactory4> mpDxgiFactory;
			Microsoft::WRL::ComPtr<ID3D12Device> mpD3d12Device;
			Microsoft::WRL::ComPtr<IDXGIDevice> mpDxgiDevice;
			Microsoft::WRL::ComPtr<ID3D12CommandQueue> mpCommandQueue;
			Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mpCommandAllocator;
			Microsoft::WRL::ComPtr<ID3D11On12Device> mpD3d11On12Device;
			Microsoft::WRL::ComPtr<ID3D11DeviceContext> mpD3d11DeviceContext;
			Microsoft::WRL::ComPtr<ID2D1Device2> mpD2d1Device;
			Microsoft::WRL::ComPtr<ID2D1DeviceContext2> mpD2d1DeviceContext;
			Microsoft::WRL::ComPtr<ID2D1Factory3> mpD2d1Factory;
			//Microsoft::WRL::ComPtr<IDWriteFactory> mpDWriteFactory;
			physx::PxPhysics* mpPhysics;
			physx::PxTolerancesScale mPxToleranceScale;
			physx::PxIonAllocatorCallback mIonAllocatorCallback;
			physx::PxIonErrorCallback mIonErrorCallback;
			void KeyboardState();
		};
	}
}
