#include "../Core/pch.h"
#include "../Core/Canvas2D.h"
#include "../Core/Application.h"

using namespace Ion::Core;

Canvas2D::Canvas2D(Window* pWindow, RECT rectangle)
	: mpWindow{ pWindow }
	, mRectangle{ rectangle }
	, mRatio{ float(rectangle.right - rectangle.left) / float(rectangle.bottom - rectangle.top) }
	, mpSwapChain{}
	, mpRenderTargets{}
	, mpWrappedBackBuffers{}
	, mpBitmaps{}
	, mpRtvHeap{}
	, mpCommandAllocator{}
	, mpGraphicsCommandList{}
	, mpBrush{}
	, mFenceEvent{}
	, mpFence{}
	, mFenceValue{ 0 }
	, mpMaterials{}
	, mRtvDescriptorSize{ 0 }
	, mViewport{ 0.f, 0.f, float(rectangle.right - rectangle.left), float(rectangle.bottom - rectangle.top), 0.f, 1.f }
	, mScissorRect{ 0, 0, rectangle.right - rectangle.left, rectangle.bottom - rectangle.top }
	, mCurrentBackBuffer{ 0 }
{
}

void Canvas2D::Initialize()
{
	Application* pApp{ mpWindow->GetApplication() };
	auto pDxgiFactory{ pApp->GetDxgiFactory() };
	auto pD2d1Factory{ pApp->GetD2d1Factory() };
	auto pDevice{ pApp->GetDevice() };
	auto pCommandQueue{ pApp->GetCommandQueue() };
	auto pCommandAllocator{ pApp->GetCommandAllocator() };
	auto pD3D11On12Device{ pApp->GetD3D11On12Device() };
	auto pD2d1DeviceContext{ pApp->GetD2d1DeviceContext() };

	// Swap Chain
	{
		Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
		DXGI_SWAP_CHAIN_DESC scd{};
		scd.BufferDesc.Width = mRectangle.right - mRectangle.left;
		scd.BufferDesc.Height = mRectangle.bottom - mRectangle.top;
		scd.BufferDesc.RefreshRate.Numerator = 60;
		scd.BufferDesc.RefreshRate.Denominator = 1;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.BufferCount = 2;
		scd.OutputWindow = mpWindow->GetHandle();
		scd.Windowed = true;
		scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		ThrowIfFailed(pDxgiFactory->CreateSwapChain(pCommandQueue.Get(), &scd, &pSwapChain));
		ThrowIfFailed(pSwapChain.As(&mpSwapChain));
	}
	mRtvDescriptorSize = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	{
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
		rtvHeapDesc.NumDescriptors = 2;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		ThrowIfFailed(pDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&mpRtvHeap)));
	}
	// RenderTarget
	{
		UINT dpi{ GetDpiForWindow(mpWindow->GetHandle()) };

		D2D1_BITMAP_PROPERTIES1 bmProp{
			D2D1::BitmapProperties1(
			D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
			float(dpi),
			float(dpi))
		};
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle{ mpRtvHeap->GetCPUDescriptorHandleForHeapStart() };
		for (size_t i{ 0 }; i < 2; ++i)
		{
			ThrowIfFailed(mpSwapChain->GetBuffer(UINT(i), IID_PPV_ARGS(&mpRenderTargets[i])));
			pDevice->CreateRenderTargetView(mpRenderTargets[i].Get(), nullptr, rtvHeapHandle);
			D3D11_RESOURCE_FLAGS d3d11Flags{ D3D11_BIND_RENDER_TARGET };
			ThrowIfFailed(pD3D11On12Device->CreateWrappedResource(
				mpRenderTargets[i].Get(),
				&d3d11Flags,
				D3D12_RESOURCE_STATE_RENDER_TARGET,
				D3D12_RESOURCE_STATE_PRESENT,
				IID_PPV_ARGS(&mpWrappedBackBuffers[i])
			));
			Microsoft::WRL::ComPtr<IDXGISurface> surface;
			ThrowIfFailed(mpWrappedBackBuffers[i].As(&surface));
			ThrowIfFailed(pD2d1DeviceContext->CreateBitmapFromDxgiSurface(
				surface.Get(),
				&bmProp,
				&mpBitmaps[i]
			));
			rtvHeapHandle.Offset(1, mRtvDescriptorSize);
			ThrowIfFailed(pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mpCommandAllocator[i])));
		}
	}
	// Graphics Command List
	{
		ThrowIfFailed(pDevice->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			mpCommandAllocator[mCurrentBackBuffer].Get(),
			nullptr,
			IID_PPV_ARGS(&mpGraphicsCommandList)));
		ThrowIfFailed(mpGraphicsCommandList->Close());
	}
	// Fence
	{
		ThrowIfFailed(pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mpFence)));
		mFenceValue = 1;
		mFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (mFenceEvent == nullptr)
			ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
		WaitForPreviousFrame();
	}
	// Brush
	{
		ThrowIfFailed(pD2d1DeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &mpBrush));
	}
}

float Canvas2D::GetRatio()
{
	return mRatio;
}

const Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>& Canvas2D::GetBrush()
{
	return mpBrush;
}

void Canvas2D::AddMaterial(Material2D* pMaterial)
{
	mpMaterials.emplace(pMaterial);
}

void Canvas2D::Render()
{
	if (mpMaterials.empty())
		return;
	Application* pApp{ mpWindow->GetApplication() };
	auto pDevice{ pApp->GetDevice() };
	auto pCmdAlloc{ pApp->GetCommandAllocator() };
	auto pCmdQueue{ pApp->GetCommandQueue() };
	auto pD2d1DeviceContext{ pApp->GetD2d1DeviceContext() };
	auto pD3D11On12Device{ pApp->GetD3D11On12Device() };
	auto pD3d11DeviceContext{ pApp->GetD3d11DeviceContext() };

	ThrowIfFailed(mpCommandAllocator[mCurrentBackBuffer]->Reset());
	ThrowIfFailed(mpGraphicsCommandList->Reset(mpCommandAllocator[mCurrentBackBuffer].Get(), nullptr));

	mpGraphicsCommandList->RSSetViewports(1, &mViewport);
	mpGraphicsCommandList->RSSetScissorRects(1, &mScissorRect);

	D3D12_RESOURCE_BARRIER rbTransition1{ CD3DX12_RESOURCE_BARRIER::Transition(
		mpRenderTargets[mCurrentBackBuffer].Get(),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET) };
	mpGraphicsCommandList->ResourceBarrier(1, &rbTransition1);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(mpRtvHeap->GetCPUDescriptorHandleForHeapStart(), mCurrentBackBuffer, mRtvDescriptorSize);
	mpGraphicsCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

	mpGraphicsCommandList->ClearRenderTargetView(rtvHandle, DirectX::Colors::Blue, 0, nullptr);

	ThrowIfFailed(mpGraphicsCommandList->Close());

	ID3D12CommandList* pCmdsLists[]{ mpGraphicsCommandList.Get() };
	pCmdQueue->ExecuteCommandLists(_countof(pCmdsLists), pCmdsLists);

	pD3D11On12Device->AcquireWrappedResources(mpWrappedBackBuffers[mCurrentBackBuffer].GetAddressOf(), 1);

	pD2d1DeviceContext->SetTarget(mpBitmaps[mCurrentBackBuffer].Get());

	pD2d1DeviceContext->BeginDraw();
	for (Material2D* pMaterial : mpMaterials)
		pMaterial->Render(this);
	ThrowIfFailed(pD2d1DeviceContext->EndDraw());

	pD3D11On12Device->ReleaseWrappedResources(mpWrappedBackBuffers[mCurrentBackBuffer].GetAddressOf(), 1);

	pD3d11DeviceContext->Flush();

	ThrowIfFailed(mpSwapChain->Present(1, 0));

	WaitForPreviousFrame();

	if (mCurrentBackBuffer == 0)
		mCurrentBackBuffer = 1;
	else
		mCurrentBackBuffer = 0;
}

void Canvas2D::WaitForPreviousFrame()
{
	Application* pApp{ mpWindow->GetApplication() };
	auto pDevice{ pApp->GetDevice() };
	auto pCommandQueue{ pApp->GetCommandQueue() };

	const UINT64 fence{ mFenceValue };

	ThrowIfFailed(pCommandQueue->Signal(mpFence.Get(), fence));

	mFenceValue++;
	if (mpFence->GetCompletedValue() < fence)
	{
		ThrowIfFailed(mpFence->SetEventOnCompletion(fence, mFenceEvent));
		WaitForSingleObject(mFenceEvent, INFINITE);
	}
}
