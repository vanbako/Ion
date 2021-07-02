#pragma once
#include "../Core/Scene.h"
#include "../Core/Material.h"
#include "../Core/Model.h"
#include "../Core/Texture.h"
#include "../Core/Window.h"

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
			Window* AddWindow(const std::wstring& title, Ion::Core::Rectangle<int> rectangle = Ion::Core::Rectangle{ 0, 0, 1280, 720 });
			LRESULT WindowsProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
			const Microsoft::WRL::ComPtr<IDXGIFactory>& GetDxgiFactory();
			const Microsoft::WRL::ComPtr<ID3D12Device>& GetDevice();
			const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& GetCommandQueue();
			const Microsoft::WRL::ComPtr<ID3D12CommandAllocator>& GetCommandAllocator();

			Material* AddMaterial(const std::string& name);
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
			std::map<std::string, Material> mMaterials;
			std::map<std::string, Model> mModels;
			std::map<std::string, Texture> mTextures;
			Microsoft::WRL::ComPtr<IDXGIFactory> mpDxgiFactory;
			Microsoft::WRL::ComPtr<ID3D12Device> mpD3d12Device;
			Microsoft::WRL::ComPtr<ID3D12CommandQueue> mpCommandQueue;
			Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mpCommandAllocator;

			void KeyboardState();
		};
	}
}
