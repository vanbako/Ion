#include "../Core/pch.h"
#include "../Core/Application.h"

using namespace Ion::Core;

LRESULT CALLBACK AppWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_CREATE)
	{
		CREATESTRUCT* pCS{ (CREATESTRUCT*)lParam };
		SetWindowLongPtr(hWnd, GWLP_USERDATA, LONG_PTR(pCS->lpCreateParams));
	}
	else
	{
		Application* pApplication{ (Application*)(GetWindowLongPtrW(hWnd, GWLP_USERDATA)) };
		if (pApplication != nullptr)
			return pApplication->WindowsProcedure(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

const std::chrono::microseconds Application::mRunSleep{ 4000 };
const std::chrono::microseconds Application::mKeyboardMutexDuration{ 1000 };

Application::Application()
	: mIsInitialized{ false }
	, mIsActive{ false }
	, mKeyboard{}
	, mKeyboardMutex{}
	, mScenes{}
	, mWindows{}
	, mMaterials3D{}
	, mMaterials2D{}
	, mModels{}
	, mTextures{}
	, mpDxgiFactory{}
	, mpD3d12Device{}
	, mpDxgiDevice{}
	, mpCommandQueue{}
	, mpCommandAllocator{}
	, mpD3d11On12Device{}
	, mpD3d11DeviceContext{}
	, mpD2d1Device{}
	, mpD2d1DeviceContext{}
	, mpD2d1Factory{}
	//, mpDWriteFactory{}
	, mpPhysics{ nullptr }
	, mPxToleranceScale{}
	, mIonAllocatorCallback{}
	, mIonErrorCallback{}
{
	WNDCLASS wndClass{};
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = AppWinProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = GetModuleHandle(nullptr);
	wndClass.hIcon = LoadIcon(0, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(0, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
	wndClass.lpszMenuName = 0;
	wndClass.lpszClassName = L"IonEngineWindowClass";
	RegisterClass(&wndClass);

	physx::PxFoundation* pFoundation{ PxCreateFoundation(PX_PHYSICS_VERSION, mIonAllocatorCallback, mIonErrorCallback) };

	mpPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *pFoundation, mPxToleranceScale, false);
}

Application::~Application()
{
	// It is important to remove the scenes, ... before the DirectX components are released
	mScenes.clear();
	mWindows.clear();
	mMaterials2D.clear();
	mMaterials3D.clear();
	mModels.clear();

	physx::PxFoundation& foundation{ mpPhysics->getFoundation() };
	mpPhysics->release();
	foundation.release();
}

bool Application::Initialize()
{
	if (!mIsInitialized)
	{
		// 3D
		// Factory
		UINT dxgiFactoryFlags{ 0 };
		UINT d3d11DeviceFlags{ D3D11_CREATE_DEVICE_BGRA_SUPPORT };
		D2D1_FACTORY_OPTIONS d2dFactoryOptions{};

#if defined(_DEBUG)
		{
			Microsoft::WRL::ComPtr<ID3D12Debug> pDebugController;
			if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&pDebugController))))
			{
				pDebugController->EnableDebugLayer();

				// Enable additional debug layers.
				dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
				d3d11DeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
				d2dFactoryOptions.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
			}
		}
#endif
		ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&mpDxgiFactory)));
		// Device
		{
			ThrowIfFailed(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&mpD3d12Device)));
		}
		// CommandQueue
		{
			D3D12_COMMAND_QUEUE_DESC queueDesc{};
			queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
			queueDesc.NodeMask = 0;
			ThrowIfFailed(mpD3d12Device->CreateCommandQueue(
				&queueDesc,
				IID_PPV_ARGS(&mpCommandQueue)));
		}
		// Command Allocator
		{
			ThrowIfFailed(mpD3d12Device->CreateCommandAllocator(
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				IID_PPV_ARGS(&mpCommandAllocator)));
		}
		// 2D & D3D11On12
		// Device & Factory
		{

			Microsoft::WRL::ComPtr<ID3D11Device> pD3d11Device{};
			ThrowIfFailed(D3D11On12CreateDevice(
				mpD3d12Device.Get(),
				d3d11DeviceFlags,
				nullptr,
				0,
				reinterpret_cast<IUnknown**>(mpCommandQueue.GetAddressOf()),
				1,
				0,
				&pD3d11Device,
				&mpD3d11DeviceContext,
				nullptr
			));
			ThrowIfFailed(pD3d11Device.As(&mpD3d11On12Device));

			D2D1_DEVICE_CONTEXT_OPTIONS deviceOptions{ D2D1_DEVICE_CONTEXT_OPTIONS_NONE };
			ThrowIfFailed(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, __uuidof(ID2D1Factory3), &d2dFactoryOptions, &mpD2d1Factory));
			ThrowIfFailed(mpD3d11On12Device.As(&mpDxgiDevice));
			ThrowIfFailed(mpD2d1Factory->CreateDevice(mpDxgiDevice.Get(), &mpD2d1Device));
			ThrowIfFailed(mpD2d1Device->CreateDeviceContext(deviceOptions, &mpD2d1DeviceContext));
			//ThrowIfFailed(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &mpDWriteFactory));
		}
		mIsInitialized = true;
	}
	return true;
}

void Application::Run()
{
	MSG msg{};
	while (msg.message != WM_QUIT)
	{
		KeyboardState();
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
			std::this_thread::sleep_for(mRunSleep);
	}
}

const bool Application::SetIsActive(bool isActive)
{
	if (mIsInitialized)
		mIsActive = isActive;
	return mIsActive;
}

const bool Application::GetIsActive() const
{
	return mIsActive;
}

bool Application::TryLockSharedKeyboard()
{
	return mKeyboardMutex.try_lock_shared_for(mKeyboardMutexDuration);
}

void Application::UnlockSharedKeyboard()
{
	mKeyboardMutex.unlock_shared();
}

PBYTE Application::GetKeyboard()
{
	return mKeyboard;
}

Scene* Application::AddScene()
{
	mScenes.emplace_back(this);
	return &mScenes.back();
}

Window* Application::AddWindow(const std::wstring& title, RECT rectangle)
{
	mWindows.emplace_back(this, title, rectangle);
	return &mWindows.back();
}

LRESULT Application::WindowsProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

const Microsoft::WRL::ComPtr<IDXGIFactory4>& Application::GetDxgiFactory()
{
	return mpDxgiFactory;
}

const Microsoft::WRL::ComPtr<ID3D12Device>& Application::GetDevice()
{
	return mpD3d12Device;
}

const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& Application::GetCommandQueue()
{
	return mpCommandQueue;
}

const Microsoft::WRL::ComPtr<ID3D12CommandAllocator>& Application::GetCommandAllocator()
{
	return mpCommandAllocator;
}

const Microsoft::WRL::ComPtr<ID2D1Factory3>& Application::GetD2d1Factory()
{
	return mpD2d1Factory;
}

const Microsoft::WRL::ComPtr<IDXGIDevice>& Application::GetDxgiDevice()
{
	return mpDxgiDevice;
}

const Microsoft::WRL::ComPtr<ID3D11On12Device>& Application::GetD3D11On12Device()
{
	return mpD3d11On12Device;
}

const Microsoft::WRL::ComPtr<ID2D1Device2>& Application::GetD2d1Device()
{
	return mpD2d1Device;
}

const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& Application::GetD3d11DeviceContext()
{
	return mpD3d11DeviceContext;
}

const Microsoft::WRL::ComPtr<ID2D1DeviceContext2>& Application::GetD2d1DeviceContext()
{
	return mpD2d1DeviceContext;
}

physx::PxPhysics* Application::GetPhysics()
{
	return mpPhysics;
}

const physx::PxTolerancesScale& Application::GetToleranceScale()
{
	return mPxToleranceScale;
}

//const Microsoft::WRL::ComPtr<IDWriteFactory>& Application::GetDWriteFactory()
//{
//	return mpDWriteFactory;
//}

Material3D* Application::AddMaterial3D(const std::string& name)
{
	auto ret{ mMaterials3D.try_emplace(name, this, name) };
	Material3D* pMaterial{ &((*(ret.first)).second) };
	pMaterial->Initialize();
	return pMaterial;
}

Material2D* Application::AddMaterial2D(const std::string& name)
{
	auto ret{ mMaterials2D.try_emplace(name, this, name) };
	Material2D* pMaterial{ &((*(ret.first)).second) };
	pMaterial->Initialize();
	return pMaterial;
}

Model* Application::AddModel(const std::string& name, Winding winding)
{
	auto ret{ mModels.try_emplace(name, this, name, winding) };
	Model* pModel{ &((*(ret.first)).second) };
	pModel->Initialize();
	return pModel;
}

Texture* Application::AddTexture(const std::string& name)
{
	auto ret{ mTextures.try_emplace(name, this, name) };
	Texture* pTexture{ &((*(ret.first)).second) };
	return pTexture;
}

void Application::KeyboardState()
{
	BOOL ret{};
	if (mKeyboardMutex.try_lock_for(mKeyboardMutexDuration))
	{
		ret = GetKeyboardState(mKeyboard);
		mKeyboardMutex.unlock();
	}
	(ret);
}
