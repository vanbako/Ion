#include "pch.h"
#include "Canvas.h"
#include "Application.h"
#include "CameraRMC.h"

using namespace Ion;

Core::Canvas::Canvas(Core::Window* pWindow, RECT rectangle)
	: mIsInitialized{ false }
	, mpWindow{ pWindow }
	, mRectangle{ rectangle }
	, mRatio{ float(rectangle.right - rectangle.left) / float(rectangle.bottom - rectangle.top) }
	, mpCamera{ nullptr }
	, mpSwapChain{}
	, mpRenderTargets{}
	, mpWrappedBackBuffers{}
	, mpDepthStencilBuffer{}
	, mpBitmaps{}
	, mpRtvHeap{}
	, mpDsvHeap{}
	, mpCanvasCbvHeap{}
	, mpGraphicsCommandList{}
	, mpBrush{}
	, mCurrentBackBuffer{ 0 }
	, mRtvDescriptorSize{ 0 }
	, mDsvDescriptorSize{ 0 }
	, mCbvDescriptorSize{ 0 }
	, mViewport{ 0.f, 0.f, float(rectangle.right - rectangle.left), float(rectangle.bottom - rectangle.top), 0.f, 1.f }
	, mScissorRect{ 0, 0, rectangle.right - rectangle.left, rectangle.bottom - rectangle.top }
	, mFenceEvent{}
	, mpFence{}
	, mFenceValue{ 0 }
	, mpCanvasConstantBuffer{}
	, mpCanvasCbvDataBegin{}
	, mCanvasConstantBufferData{}
	, mpMaterials3D{}
	, mpMaterials2D{}
	, mThread{}
	, mpMutex{ nullptr }
	, mpConditionVar{ nullptr }
	, mThreadAction{}
	, mRunThread{ false }
{
	if (mThread.get_id() == std::thread::id{})
		mThread = std::thread{ &ThreadRender, this };
}

Core::Canvas::~Canvas()
{
	if (mThread.joinable())
		mThread.join();
}

void Core::Canvas::Initialize()
{
	Core::Application* pApp{ mpWindow->GetApplication() };
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
		mpWindow->GetApplication()->ThrowIfFailed(pDxgiFactory->CreateSwapChain(pCommandQueue.Get(), &scd, &pSwapChain));
		mpWindow->GetApplication()->ThrowIfFailed(pSwapChain.As(&mpSwapChain));
	}

	// Descriptor Heaps
	mRtvDescriptorSize = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	{
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
		rtvHeapDesc.NumDescriptors = 2;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		rtvHeapDesc.NodeMask = 0;
		mpWindow->GetApplication()->ThrowIfFailed(pDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&mpRtvHeap)));
	}
	mDsvDescriptorSize = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	{
		D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
		dsvHeapDesc.NumDescriptors = 1;
		dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		dsvHeapDesc.NodeMask = 0;
		mpWindow->GetApplication()->ThrowIfFailed(pDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&mpDsvHeap)));
	}
	mCbvDescriptorSize = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	{
		D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc{};
		cbvHeapDesc.NumDescriptors = 1;
		cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		mpWindow->GetApplication()->ThrowIfFailed(pDevice->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&mpCanvasCbvHeap)));
	}

	// Render Targets
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
			mpWindow->GetApplication()->ThrowIfFailed(mpSwapChain->GetBuffer(UINT(i), IID_PPV_ARGS(&mpRenderTargets[i])));
			pDevice->CreateRenderTargetView(mpRenderTargets[i].Get(), nullptr, rtvHeapHandle);
			D3D11_RESOURCE_FLAGS d3d11Flags{ D3D11_BIND_RENDER_TARGET };
			mpWindow->GetApplication()->ThrowIfFailed(pD3D11On12Device->CreateWrappedResource(
				mpRenderTargets[i].Get(),
				&d3d11Flags,
				D3D12_RESOURCE_STATE_RENDER_TARGET,
				D3D12_RESOURCE_STATE_PRESENT,
				IID_PPV_ARGS(&mpWrappedBackBuffers[i])
			));
			Microsoft::WRL::ComPtr<IDXGISurface> surface;
			mpWindow->GetApplication()->ThrowIfFailed(mpWrappedBackBuffers[i].As(&surface));
			mpWindow->GetApplication()->ThrowIfFailed(pD2d1DeviceContext->CreateBitmapFromDxgiSurface(
				surface.Get(),
				&bmProp,
				&mpBitmaps[i]
			));
			rtvHeapHandle.Offset(1, mRtvDescriptorSize);
			//mpWindow->GetApplication()->ThrowIfFailed(pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mpCommandAllocator[i])));
		}
	}

	// Depth Stencil
	{
		D3D12_RESOURCE_DESC dsDesc{};
		dsDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		dsDesc.Alignment = 0;
		dsDesc.Width = mRectangle.right - mRectangle.left;;
		dsDesc.Height = mRectangle.bottom - mRectangle.top;
		dsDesc.DepthOrArraySize = 1;
		dsDesc.MipLevels = 1;
		dsDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dsDesc.SampleDesc.Count = 1;
		dsDesc.SampleDesc.Quality = 0;
		dsDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		dsDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		D3D12_CLEAR_VALUE clearValue{};
		clearValue.Format = DXGI_FORMAT_D32_FLOAT;
		clearValue.DepthStencil.Depth = 1.0f;
		clearValue.DepthStencil.Stencil = 0;
		D3D12_HEAP_PROPERTIES heapProps{ CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT) };
		mpWindow->GetApplication()->ThrowIfFailed(pDevice->CreateCommittedResource(
			&heapProps,
			D3D12_HEAP_FLAG_NONE,
			&dsDesc,
			//D3D12_RESOURCE_STATE_COMMON,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&clearValue,
			IID_PPV_ARGS(mpDepthStencilBuffer.GetAddressOf())));
		pDevice->CreateDepthStencilView(mpDepthStencilBuffer.Get(), nullptr, mpDsvHeap->GetCPUDescriptorHandleForHeapStart());
	}

	// Constant Buffer
	{
		const UINT canvasConstantBufferSize{ sizeof(CanvasConstantBuffer) };

		D3D12_HEAP_PROPERTIES heapProp{ CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD) };
		D3D12_RESOURCE_DESC resDesc{ CD3DX12_RESOURCE_DESC::Buffer(canvasConstantBufferSize) };
		mpWindow->GetApplication()->ThrowIfFailed(pDevice->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mpCanvasConstantBuffer)));

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
		cbvDesc.BufferLocation = mpCanvasConstantBuffer->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = canvasConstantBufferSize;
		pDevice->CreateConstantBufferView(&cbvDesc, mpCanvasCbvHeap->GetCPUDescriptorHandleForHeapStart());

		CD3DX12_RANGE readRange(0, 0);
		mpWindow->GetApplication()->ThrowIfFailed(mpCanvasConstantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&mpCanvasCbvDataBegin)));
		DirectX::XMStoreFloat4x4(&mCanvasConstantBufferData.mView, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&mCanvasConstantBufferData.mViewProj, DirectX::XMMatrixIdentity());
		mCanvasConstantBufferData.mLightDirection = DirectX::XMFLOAT3{ -0.577f, -0.577f, 0.577f };
		mCanvasConstantBufferData.mColorDiffuse = DirectX::XMFLOAT4{ 1.f, 1.f, 1.f, 1.f };
		mCanvasConstantBufferData.mColorAmbient = DirectX::XMFLOAT4{ 1.f, 1.f, 1.f, 1.f };
		mCanvasConstantBufferData.mAmbientIntensity = 0.15f;
		memcpy(mpCanvasCbvDataBegin, &mCanvasConstantBufferData, canvasConstantBufferSize);
	}
	// Graphics Command List
	{
		mpWindow->GetApplication()->ThrowIfFailed(pDevice->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			pCommandAllocator.Get(),
			nullptr,
			IID_PPV_ARGS(&mpGraphicsCommandList)));
		mpGraphicsCommandList->Close();
	}
	// Fence
	{
		mpWindow->GetApplication()->ThrowIfFailed(pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mpFence)));
		mFenceValue = 1;
		mFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (mFenceEvent == nullptr)
			mpWindow->GetApplication()->ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
		WaitForPreviousFrame();
	}
	// Brush
	{
		mpWindow->GetApplication()->ThrowIfFailed(pD2d1DeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &mpBrush));
	}
	mIsInitialized = true;
}

void Core::Canvas::SetCamera(Core::Object* pCamera)
{
	mpCamera = pCamera;
	if (pCamera != nullptr)
	pCamera->GetModelC<CameraRMC>()->SetCanvas(this);
}

Core::Object* Core::Canvas::GetCamera()
{
	return mpCamera;
}

float Core::Canvas::GetRatio()
{
	return mRatio;
}

const Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>& Core::Canvas::GetBrush()
{
	return mpBrush;
}

D3D12_CPU_DESCRIPTOR_HANDLE Core::Canvas::GetCurrentBackBufferView()
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE{
		mpRtvHeap->GetCPUDescriptorHandleForHeapStart(),
		mCurrentBackBuffer,
		mRtvDescriptorSize };
}

Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& Core::Canvas::GetGraphicsCommandList()
{
	return mpGraphicsCommandList;
}

void Core::Canvas::AddMaterial(Core::Material3D* pMaterial)
{
	mpMaterials3D.emplace(pMaterial);
}

void Core::Canvas::AddMaterial(Core::Material2D* pMaterial)
{
	mpMaterials2D.emplace(pMaterial);
}

void Core::Canvas::SetDescriptor()
{
	ID3D12DescriptorHeap* ppHeaps[]{ mpCanvasCbvHeap.Get() };
	mpGraphicsCommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	mpGraphicsCommandList->SetGraphicsRootDescriptorTable(0, mpCanvasCbvHeap->GetGPUDescriptorHandleForHeapStart());
}

void Core::Canvas::Render()
{
#ifdef _DEBUG
	if (!mIsInitialized)
	{
		mpWindow->GetApplication()->GetServiceLocator().GetLogger()->Message(this, Core::MsgType::Fatal, "Canvas.Render() while mIsInitialized == false");
		return;
	}
#endif
	if ((mpCamera == nullptr) && (!mpMaterials3D.empty()))
		return;
	if (mpMaterials2D.empty() && mpMaterials3D.empty())
		return;
	Application* pApp{ mpWindow->GetApplication() };
	auto pDevice{ pApp->GetDevice() };
	auto pCmdAlloc{ pApp->GetCommandAllocator() };
	auto pCmdQueue{ pApp->GetCommandQueue() };
	auto pD2d1DeviceContext{ pApp->GetD2d1DeviceContext() };
	auto pD3D11On12Device{ pApp->GetD3D11On12Device() };
	auto pD3d11DeviceContext{ pApp->GetD3d11DeviceContext() };

	mpWindow->GetApplication()->ThrowIfFailed(pCmdAlloc->Reset());
	mpWindow->GetApplication()->ThrowIfFailed(mpGraphicsCommandList->Reset(pCmdAlloc.Get(), nullptr));

	if (!mpMaterials3D.empty())
	{
		mCanvasConstantBufferData.mView = mpCamera->GetModelC<CameraRMC>()->GetView();
		mCanvasConstantBufferData.mViewProj = mpCamera->GetModelC<CameraRMC>()->GetViewProjection();
		memcpy(mpCanvasCbvDataBegin, &mCanvasConstantBufferData, sizeof(Core::CanvasConstantBuffer));
	}

	mpGraphicsCommandList->RSSetViewports(1, &mViewport);
	mpGraphicsCommandList->RSSetScissorRects(1, &mScissorRect);

	D3D12_RESOURCE_BARRIER rbTransition1{ CD3DX12_RESOURCE_BARRIER::Transition(
		mpRenderTargets[mCurrentBackBuffer].Get(),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET) };
	mpGraphicsCommandList->ResourceBarrier(1, &rbTransition1);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(mpRtvHeap->GetCPUDescriptorHandleForHeapStart(), mCurrentBackBuffer, mRtvDescriptorSize);
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle{ mpDsvHeap->GetCPUDescriptorHandleForHeapStart() };
	mpGraphicsCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

	mpGraphicsCommandList->ClearRenderTargetView(rtvHandle, DirectX::Colors::Black, 0, nullptr);
	mpGraphicsCommandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	for (Material3D* pMaterial : mpMaterials3D)
		pMaterial->Render(this);

	if (mpMaterials2D.empty())
	{
		D3D12_RESOURCE_BARRIER rbTransition2{ CD3DX12_RESOURCE_BARRIER::Transition(
			mpRenderTargets[mCurrentBackBuffer].Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT) };
		mpGraphicsCommandList->ResourceBarrier(1, &rbTransition2);
		mpWindow->GetApplication()->ThrowIfFailed(mpGraphicsCommandList->Close());
		ID3D12CommandList* pCmdsLists[]{ mpGraphicsCommandList.Get() };
		pCmdQueue->ExecuteCommandLists(_countof(pCmdsLists), pCmdsLists);
	}
	else
	{
		mpWindow->GetApplication()->ThrowIfFailed(mpGraphicsCommandList->Close());
		ID3D12CommandList* pCmdsLists[]{ mpGraphicsCommandList.Get() };
		pCmdQueue->ExecuteCommandLists(_countof(pCmdsLists), pCmdsLists);
		pD3D11On12Device->AcquireWrappedResources(mpWrappedBackBuffers[mCurrentBackBuffer].GetAddressOf(), 1);
		pD2d1DeviceContext->SetTarget(mpBitmaps[mCurrentBackBuffer].Get());
		pD2d1DeviceContext->BeginDraw();
		for (Material2D* pMaterial : mpMaterials2D)
			pMaterial->Render(this);
		mpWindow->GetApplication()->ThrowIfFailed(pD2d1DeviceContext->EndDraw());
		pD3D11On12Device->ReleaseWrappedResources(mpWrappedBackBuffers[mCurrentBackBuffer].GetAddressOf(), 1);
		pD3d11DeviceContext->Flush();
	}

	mpWindow->GetApplication()->ThrowIfFailed(mpSwapChain->Present(1, 0));

	WaitForPreviousFrame();

	if (mCurrentBackBuffer == 0)
		mCurrentBackBuffer = 1;
	else
		mCurrentBackBuffer = 0;
}

void Core::Canvas::WaitForPreviousFrame()
{
	Core::Application* pApp{ mpWindow->GetApplication() };
	auto pDevice{ pApp->GetDevice() };
	auto pCommandQueue{ pApp->GetCommandQueue() };

	const UINT64 fence{ mFenceValue };

	mpWindow->GetApplication()->ThrowIfFailed(pCommandQueue->Signal(mpFence.Get(), fence));

	mFenceValue++;
	if (mpFence->GetCompletedValue() < fence)
	{
		mpWindow->GetApplication()->ThrowIfFailed(mpFence->SetEventOnCompletion(fence, mFenceEvent));
		WaitForSingleObject(mFenceEvent, INFINITE);
	}
}

void Core::Canvas::RunThread(std::condition_variable* pConditionVar, std::mutex* pMutex)
{
	if (mRunThread.load())
		return;
	mpConditionVar = pConditionVar;
	mpMutex = pMutex;
	mRunThread.store(true);
}

void Core::Canvas::SetThreadAction(Core::ThreadAction threadAction)
{
	mThreadAction = threadAction;
}

void Core::Canvas::ThreadRender(Core::Canvas* pCanvas)
{
	bool close{ false };
	while (!close)
	{
		if (!pCanvas->mRunThread.load())
			std::this_thread::sleep_for(std::chrono::milliseconds{ 16 });
		std::unique_lock<std::mutex> lk{ *pCanvas->mpMutex };
		pCanvas->mpConditionVar->wait(lk);
		switch (pCanvas->mThreadAction)
		{
		case Core::ThreadAction::Render:
			pCanvas->Render();
			break;
		case Core::ThreadAction::Close:
			close = true;
			break;
		}
		pCanvas->mThreadAction = Core::ThreadAction::Sleep;
		lk.unlock();
	}
}
