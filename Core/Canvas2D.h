#pragma once
#include "../Core/d3dx12.h"

namespace Ion
{
	namespace Core
	{
		class Object;
		class Window;
		class Material2D;

		class Canvas2D final
		{
		public:
			explicit Canvas2D(Window* pWindow, RECT rectangle);
			~Canvas2D() = default;
			Canvas2D(const Canvas2D& other) = delete;
			Canvas2D(Canvas2D&& other) noexcept = delete;
			Canvas2D& operator=(const Canvas2D& other) = delete;
			Canvas2D& operator=(Canvas2D&& other) noexcept = delete;

			void Initialize();
			float GetRatio();
			const Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>& GetBrush();

			void AddMaterial(Material2D* pMaterial);
			void Render();
			void WaitForPreviousFrame();
		private:
			Window* mpWindow;
			RECT mRectangle;
			float mRatio;
			Microsoft::WRL::ComPtr<IDXGISwapChain3> mpSwapChain;
			Microsoft::WRL::ComPtr<ID3D12Resource> mpRenderTargets[2];
			Microsoft::WRL::ComPtr<ID3D11Resource> mpWrappedBackBuffers[2];
			Microsoft::WRL::ComPtr<ID2D1Bitmap1> mpBitmaps[2];
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mpRtvHeap;
			Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mpCommandAllocator[2];
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mpGraphicsCommandList;
			Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> mpBrush;
			HANDLE mFenceEvent;
			Microsoft::WRL::ComPtr<ID3D12Fence> mpFence;
			UINT64 mFenceValue;
			std::set<Material2D*> mpMaterials;
			UINT mRtvDescriptorSize;
			D3D12_VIEWPORT mViewport;
			CD3DX12_RECT mScissorRect;
			int mCurrentBackBuffer;
		};
	}
}
