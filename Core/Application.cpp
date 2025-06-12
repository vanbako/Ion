#include "pch.h"
#include "Application.h"
#include "TextureResource.h"
#include "MeshModelResource.h"
#include "Material2DResource.h"
#include "Material3DResource.h"
#include "Logger.h"
#include <timeapi.h>

using namespace Ion;

LRESULT CALLBACK Core::AppWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_CREATE)
	{
		CREATESTRUCT* pCS{ (CREATESTRUCT*)lParam };
		SetWindowLongPtr(hWnd, GWLP_USERDATA, LONG_PTR(pCS->lpCreateParams));
	}
	else
	{
		Core::Application* pApplication{ (Core::Application*)(GetWindowLongPtrW(hWnd, GWLP_USERDATA)) };
		if (pApplication != nullptr)
			return pApplication->WindowsProcedure(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

const std::chrono::milliseconds Core::Application::mRunSleep{ 4 };
const std::chrono::milliseconds Core::Application::mKeyboardMutexDuration{ 1 };

Core::Application::Application()
	: mIsInitialized{ false }
	, mIsActive{ false }
	, mCursorMode{ CursorMode::Absolute }
	, mKeyboard{}
	, mKeyboardMutex{}
	, mCursor{}
	, mCursorSavePos{}
	, mScenes{}
	, mWindows{}
	, mResourceManager{ this }
	, mpDxgiFactory{}
	, mpD3d12Device{}
	, mpDxgiDevice{}
	, mpCommandQueue{}
	, mpPhysics{ nullptr }
	, mPxToleranceScale{}
	, mIonAllocatorCallback{}
	, mIonErrorCallback{}
	, mpCooking{ nullptr }
	, mServiceLocator{}
	, mHideCursor{ false }
	, mShowCursor{ false }
{
	mResourceManager.AddResource<TextureResource>();
	mResourceManager.AddResource<MeshModelResource>();
	mResourceManager.AddResource<Material2DResource>();
	mResourceManager.AddResource<Material3DResource>();
	WNDCLASS wndClass{};
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = Core::AppWinProc;
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
	mpCooking = PxCreateCooking(PX_PHYSICS_VERSION, *pFoundation, physx::PxCookingParams{ mPxToleranceScale });
	PxRegisterHeightFields(*mpPhysics);
	POINT pos{ 0, 0 };
	GetCursorPos(&pos);
	mCursorSavePos.store(pos);
}

Core::Application::~Application()
{
	// It is important to remove the scenes, ... before the DirectX components are released
	mScenes.clear();
	for (auto& canvas : mCanvases)
		canvas.WaitThreadEnd();
	mCanvases.clear();
	mWindows.clear();
	//mResourceManager.Clear();

	mpCooking->release();
	physx::PxFoundation& foundation{ mpPhysics->getFoundation() };
	mpPhysics->release();
	foundation.release();
}

void Core::Application::ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
#ifdef ION_LOGGER
		mServiceLocator.GetLogger()->Message(typeid(this).name(), Core::MsgType::Fatal, "DirectX Exception " + std::to_string(hr));
#endif
		throw HrException(hr);
	}
}

bool Core::Application::Initialize()
{
	if (mIsInitialized)
		return true;

	UINT dxgiFactoryFlags{ 0 };
#ifdef _DEBUG
	{
		Microsoft::WRL::ComPtr<ID3D12Debug> pDebugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&pDebugController))))
		{
			pDebugController->EnableDebugLayer();
			dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
		}
	}
#endif
	ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&mpDxgiFactory)));
        // Device
        {
                ThrowIfFailed(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&mpD3d12Device)));
        }
        // Global descriptor heap
        {
                mGlobalCbvSrvDescriptorSize = mpD3d12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
                D3D12_DESCRIPTOR_HEAP_DESC desc{};
                desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
                desc.NumDescriptors = 100000;
                desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
                ThrowIfFailed(mpD3d12Device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&mpGlobalCbvSrvHeap)));
                mNextDescriptorIndex = 0;
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
	mIsInitialized = true;
#ifdef ION_LOGGER
	mServiceLocator.GetLogger()->Message(typeid(this).name(), MsgType::Info, "Application Initialized");
#endif
	return true;
}

void Core::Application::Run()
{
	timeBeginPeriod(1);
	MSG msg{};
	while (msg.message != WM_QUIT)
	{
		KeyboardState();
		if (mHideCursor)
		{
			while (ShowCursor(FALSE) > 0);
			mHideCursor.store(false);
		}
		if (mShowCursor)
		{
			ShowCursor(TRUE);
			mShowCursor.store(false);
		}
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
			std::this_thread::sleep_for(mRunSleep);
	}
	timeEndPeriod(1);
}

const bool Core::Application::SetIsActive(bool isActive)
{
	if (mIsInitialized)
		mIsActive = isActive;
	return mIsActive;
}

const bool Core::Application::GetIsActive() const
{
	return mIsActive;
}

Core::CursorMode Core::Application::GetCursorMode() const
{
	return mCursorMode;
}

void Core::Application::SetCursorMode(Core::CursorMode cursorMode)
{
	if (cursorMode == GetCursorMode())
		return;
	POINT pos{ 0, 0 };
	switch (cursorMode)
	{
	case Core::CursorMode::Relative:
		GetCursorPos(&pos);
		mCursorSavePos.store(pos);
		mHideCursor.store(true);
		break;
	case Core::CursorMode::Absolute:
		pos = mCursorSavePos.load();
		SetCursorPos(pos.x, pos.y);
		mShowCursor.store(true);
		break;
	}
	mCursorMode = cursorMode;
}

bool Core::Application::TryLockSharedKeyboard()
{
	return mKeyboardMutex.try_lock_shared_for(mKeyboardMutexDuration);
}

void Core::Application::UnlockSharedKeyboard()
{
	mKeyboardMutex.unlock_shared();
}

PBYTE Core::Application::GetKeyboard()
{
	return mKeyboard;
}

POINT Core::Application::GetCursorPosition()
{
	POINT pos{ 0, 0 };
	GetCursorPos(&pos);
	if (mCursorMode == Core::CursorMode::Relative)
	{
		POINT savePos{ mCursorSavePos.load() };
		SetCursorPos(savePos.x, savePos.y);
		pos.x = savePos.x - pos.x;
		pos.y = savePos.y - pos.y;
	}
	return pos;
}

Core::Scene* Core::Application::AddScene(const std::string& name)
{
	return &mScenes.emplace_back(name, this);
}

Core::Scene* Core::Application::GetScene(std::size_t num)
{
#ifdef _DEBUG
	if (num >= mScenes.size())
	{
#ifdef ION_LOGGER
		GetServiceLocator().GetLogger()->Message(typeid(this).name(), Core::MsgType::Fatal, "Core::Application::GetScene, num >= mScenes.size()");
#endif
	}
#endif
	auto it{ mScenes.begin() };
	for (std::size_t i{ 0 }; i < num; ++i)
		++it;
	return &(*it);
}

Core::Window* Core::Application::AddWindow(const std::wstring& title, RECT rectangle)
{
	return &mWindows.emplace_back(this, title, rectangle);
}

Core::Canvas* Core::Application::AddCanvas(RECT rectangle)
{
	return &mCanvases.emplace_back(this, rectangle);
}

std::list<Core::Canvas>& Ion::Core::Application::GetCanvases()
{
	return mCanvases;
}

LRESULT Core::Application::WindowsProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

const Microsoft::WRL::ComPtr<IDXGIFactory5>& Core::Application::GetDxgiFactory()
{
	return mpDxgiFactory;
}

const Microsoft::WRL::ComPtr<ID3D12Device5>& Core::Application::GetDevice()
{
	return mpD3d12Device;
}

const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& Core::Application::GetCommandQueue()
{
        return mpCommandQueue;
}

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& Core::Application::GetGlobalCbvSrvHeap()
{
        return mpGlobalCbvSrvHeap;
}

UINT Core::Application::AllocateDescriptors(UINT count)
{
        UINT index{ mNextDescriptorIndex };
        mNextDescriptorIndex += count;
        return index;
}

D3D12_CPU_DESCRIPTOR_HANDLE Core::Application::GetCpuHandle(UINT index)
{
        return CD3DX12_CPU_DESCRIPTOR_HANDLE(mpGlobalCbvSrvHeap->GetCPUDescriptorHandleForHeapStart(), index, mGlobalCbvSrvDescriptorSize);
}

D3D12_GPU_DESCRIPTOR_HANDLE Core::Application::GetGpuHandle(UINT index)
{
        return CD3DX12_GPU_DESCRIPTOR_HANDLE(mpGlobalCbvSrvHeap->GetGPUDescriptorHandleForHeapStart(), index, mGlobalCbvSrvDescriptorSize);
}

physx::PxPhysics* Core::Application::GetPxPhysics()
{
	return mpPhysics;
}

physx::PxCooking* Core::Application::GetPxCooking()
{
	return mpCooking;
}

const physx::PxTolerancesScale& Core::Application::GetToleranceScale()
{
	return mPxToleranceScale;
}

Core::ServiceLocator& Core::Application::GetServiceLocator()
{
	return mServiceLocator;
}

Core::ResourceManager* Core::Application::GetResourceManager()
{
	return &mResourceManager;
}

void Core::Application::KeyboardState()
{
	BOOL ret{};
	if (mKeyboardMutex.try_lock_for(mKeyboardMutexDuration))
	{
		ret = GetKeyboardState(mKeyboard);
		mKeyboardMutex.unlock();
	}
	(ret);
}
