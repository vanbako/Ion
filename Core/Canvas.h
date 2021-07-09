#pragma once
#include "../Core/CanvasConstantBuffer.h"
#include "../Core/ThreadAction.h"
#include "../Core/d3dx12.h"

namespace Ion
{
	namespace Core
	{
		class Object;
		class Window;
		class Material3D;
		class Material2D;

		class Canvas final
		{
		public:
			explicit Canvas(Window* pWindow, RECT rectangle);
			~Canvas();
			Canvas(const Canvas& other) = delete;
			Canvas(Canvas&& other) noexcept = delete;
			Canvas& operator=(const Canvas& other) = delete;
			Canvas& operator=(Canvas&& other) noexcept = delete;

			void Initialize();
			void SetCamera(Object* pCamera);
			Object* GetCamera();
			float GetRatio();
			const Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>& GetBrush();

			D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentBackBufferView();
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& GetGraphicsCommandList();

			void AddMaterial(Material3D* pMaterial);
			void AddMaterial(Material2D* pMaterial);
			void SetDescriptor();
			void WaitForPreviousFrame();

			void RunThread(std::condition_variable* pConditionVar, std::mutex* pMutex);
			void SetThreadAction(ThreadAction threadAction);
		private:
			Window* mpWindow;
			RECT mRectangle;
			float mRatio;
			Object* mpCamera;
			Microsoft::WRL::ComPtr<IDXGISwapChain3> mpSwapChain;
			Microsoft::WRL::ComPtr<ID3D12Resource>
				mpRenderTargets[2],
				mpDepthStencilBuffer;
			Microsoft::WRL::ComPtr<ID3D11Resource> mpWrappedBackBuffers[2];
			Microsoft::WRL::ComPtr<ID2D1Bitmap1> mpBitmaps[2];
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>
				mpRtvHeap,
				mpDsvHeap,
				mpCanvasCbvHeap;
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mpGraphicsCommandList;
			Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> mpBrush;
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
			CanvasConstantBuffer mCanvasConstantBufferData;
			std::set<Material3D*> mpMaterials3D;
			std::set<Material2D*> mpMaterials2D;
			std::thread mThread;
			std::mutex* mpMutex;
			std::condition_variable* mpConditionVar;
			ThreadAction mThreadAction;
			std::atomic<bool> mRunThread;

			void Render();
			static void ThreadRender(Canvas* pCanvas);
		};
	}
}
