#pragma once
#include "CanvasConstantBuffer.h"
#include "ThreadAction.h"

namespace Ion
{
	namespace Core
	{
		class Application;
		class Object;
		class Window;
		class Material3D;
		class Material2D;

		class Canvas final
		{
		public:
			explicit Canvas(Core::Application* pApplication, RECT rectangle);
			~Canvas();
			Canvas(const Canvas& other) = delete;
			Canvas(Canvas&& other) noexcept = delete;
			Canvas& operator=(const Canvas& other) = delete;
			Canvas& operator=(Canvas&& other) noexcept = delete;

			void Initialize();
			void SetWindow(Core::Window* pWindow);
			void SetCamera(Core::Object* pCamera);
			Core::Object* GetCamera();
			float GetRatio();

			D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentBackBufferView();
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList5>& GetGraphicsCommandList();

			void AddMaterial(Core::Material3D* pMaterial);
			void AddMaterial(Core::Material2D* pMaterial);
			void SetDescriptor();
			void WaitForPreviousFrame();

			void RunThread(std::condition_variable* pConditionVar, std::mutex* pMutex);
			void SetThreadAction(Core::ThreadAction threadAction);
			void WaitThreadEnd();
			void ViewCUpdate(float delta);
		private:
			static const std::size_t mBackBufferCount{ 2 };
			bool mIsInitialized;
			Core::Application* mpApplication;
			Core::Window* mpWindow;
			RECT mRectangle;
			float mRatio;
			Core::Object* mpCamera;
			Microsoft::WRL::ComPtr<IDXGISwapChain4> mpSwapChain;
			Microsoft::WRL::ComPtr<ID3D12Resource>
				mpRenderTargets[mBackBufferCount],
				mpDepthStencilBuffer;
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>
				mpRtvHeap,
				mpDsvHeap,
				mpCanvasCbvHeap;
			Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mpCommandAllocator;
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList5> mpGraphicsCommandList;
			int mCurrentBackBuffer;
			UINT
				mRtvDescriptorSize,
				mDsvDescriptorSize,
				mCbvDescriptorSize;
			D3D12_VIEWPORT mViewport;
			CD3DX12_RECT mScissorRect;
			HANDLE mFenceEvent;
			Microsoft::WRL::ComPtr<ID3D12Fence> mpFence;
			UINT64 mFenceValue;
			Microsoft::WRL::ComPtr<ID3D12Resource> mpCanvasConstantBuffer;
			UINT8* mpCanvasCbvDataBegin;
			Core::CanvasConstantBuffer mCanvasConstantBufferData;
			std::set<Core::Material3D*> mpMaterials3D;
			std::set<Core::Material2D*> mpMaterials2D;
			std::thread mThread;
			std::mutex* mpMutex;
			std::condition_variable* mpConditionVar;
			std::atomic<Core::ThreadAction> mThreadAction;
			std::atomic<bool> mRunThread;
			ID2D1Multithread* mpD2DMultithread;

			void Render();
			static void ThreadRender(Core::Canvas* pCanvas);
		};
	}
}
