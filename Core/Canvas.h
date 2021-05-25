#pragma once
#include "../Core/Rectangle.h"
#include "../Core/CanvasConstantBuffer.h"
#include "../Core/UploadBuffer.h"

namespace Ion
{
	namespace Core
	{
		class Object;
		class Window;
		class Material;

		class Canvas final
		{
		public:
			explicit Canvas(Window* pWindow, Ion::Core::Rectangle<int> rectangle);
			~Canvas() = default;
			Canvas(const Canvas& other) = delete;
			Canvas(Canvas&& other) noexcept = delete;
			Canvas& operator=(const Canvas& other) = delete;
			Canvas& operator=(Canvas&& other) noexcept = delete;

			void Initialize();
			void SetCamera(Object* pCamera);
			Object* GetCamera();
			float GetRatio();

			D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentBackBufferView();
			D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilView();
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& GetGraphicsCommandList();

			void AddMaterial(Material* pMaterial);
			void SetDescriptor();
			void Render();
			void WaitForPreviousFrame();
		private:
			Window* mpWindow;
			Ion::Core::Rectangle<int> mRectangle;
			float mRatio;
			Object* mpCamera;
			Microsoft::WRL::ComPtr<IDXGISwapChain> mpSwapChain;
			Microsoft::WRL::ComPtr<ID3D12Resource>
				mpRenderTargets[2],
				mpDepthStencilBuffer;
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>
				mpRtvHeap,
				mpDsvHeap,
				mpCanvasCbvHeap;
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mpGraphicsCommandList;
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
			std::set<Material*> mpMaterials;
		};
	}
}
