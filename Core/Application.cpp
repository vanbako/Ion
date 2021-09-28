#include "pch.h"
#include "Application.h"
#include "Logger.h"
#include <timeapi.h>

using namespace Ion;

LRESULT CALLBACK AppWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
const std::chrono::milliseconds Core::Application::mTextureMutexDuration{ 100 };
const std::chrono::milliseconds Core::Application::mModelMutexDuration{ 100 };
const std::chrono::milliseconds Core::Application::mMaterialMutexDuration{ 100 };

Core::Application::Application()
	: mIsInitialized{ false }
	, mIsActive{ false }
	, mKeyboard{}
	, mKeyboardMutex{}
	, mTextureMutex{}
	, mMaterialMutex{}
	, mModelMutex{}
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
	, mpPhysics{ nullptr }
	, mPxToleranceScale{}
	, mIonAllocatorCallback{}
	, mIonErrorCallback{}
	, mpCooking{ nullptr }
	, mServiceLocator{}
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
	mpCooking = PxCreateCooking(PX_PHYSICS_VERSION, *pFoundation, physx::PxCookingParams{ mPxToleranceScale });
	PxRegisterHeightFields(*mpPhysics);
}

Core::Application::~Application()
{
	// It is important to remove the scenes, ... before the DirectX components are released
	mScenes.clear();
	mWindows.clear();
	mMaterials2D.clear();
	mMaterials3D.clear();
	mModels.clear();

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

Core::Scene* Core::Application::AddScene(const std::string& name)
{
	return &mScenes.emplace_back(name, this);
}

Core::Scene* Core::Application::GetScene(size_t num)
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
	for (size_t i{ 0 }; i < num; ++i)
		++it;
	return &(*it);
}

Core::Window* Core::Application::AddWindow(const std::wstring& title, RECT rectangle)
{
	return &mWindows.emplace_back(this, title, rectangle);
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

physx::PxPhysics* Core::Application::GetPxPhysics()
{
	return mpPhysics;
}

physx::PxCooking* Ion::Core::Application::GetPxCooking()
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

Core::Material3D* Core::Application::AddMaterial3D(const std::string& name)
{
	if (!mMaterialMutex.try_lock_for(mMaterialMutexDuration))
	{
#ifdef ION_LOGGER
		GetServiceLocator().GetLogger()->Message(typeid(this).name(), Core::MsgType::Fatal, "Core::Application::AddMaterial3D failed, could not get lock");
#endif
		return nullptr;
	}
	auto ret{ mMaterials3D.try_emplace(name, this, name) };
	Material3D* pMaterial{ &((*(ret.first)).second) };
	pMaterial->Initialize();
	mMaterialMutex.unlock();
	return pMaterial;
}

Core::Material2D* Core::Application::AddMaterial2D(const std::string& name)
{
	if (!mMaterialMutex.try_lock_for(mMaterialMutexDuration))
	{
#ifdef ION_LOGGER
		GetServiceLocator().GetLogger()->Message(typeid(this).name(), Core::MsgType::Fatal, "Core::Application::AddMaterial2D failed, could not get lock");
#endif
		return nullptr;
	}
	auto ret{ mMaterials2D.try_emplace(name, this, name) };
	Material2D* pMaterial{ &((*(ret.first)).second) };
	pMaterial->Initialize();
	mMaterialMutex.unlock();
	return pMaterial;
}

Core::Model* Core::Application::AddModel(const std::string& fileName, const std::string& fileExtension, Winding winding, CoordSystem coordSystem)
{
	if (!mModelMutex.try_lock_for(mModelMutexDuration))
	{
#ifdef ION_LOGGER
		GetServiceLocator().GetLogger()->Message(typeid(this).name(), Core::MsgType::Fatal, "Core::Application::AddModel failed, could not get lock");
#endif
		return nullptr;
	}
	auto ret{ mModels.try_emplace(fileName, this, fileName, fileExtension, winding, coordSystem) };
	Model* pModel{ &((*(ret.first)).second) };
	pModel->Initialize();
	mModelMutex.unlock();
	return pModel;
}

Core::Texture* Core::Application::AddTexture(const std::string& name)
{
	if (!mTextureMutex.try_lock_for(mTextureMutexDuration))
	{
#ifdef ION_LOGGER
		GetServiceLocator().GetLogger()->Message(typeid(this).name(), Core::MsgType::Fatal, "Core::Application::AddTexture failed, could not get lock");
#endif
		return nullptr;
	}
	auto ret{ mTextures.try_emplace(name, this, name) };
	Texture* pTexture{ &((*(ret.first)).second) };
	mTextureMutex.unlock();
	return pTexture;
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
