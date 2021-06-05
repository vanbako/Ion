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

Application::Application()
	: mIsInitialized{ false }
	, mIsActive{ false }
	, mScenes{}
	, mWindows{}
	, mMaterials{}
	, mpDxgiFactory{}
	, mpD3d12Device{ }
	, mpCommandQueue{}
	, mpCommandAllocator{}
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
}

Application::~Application()
{
	mScenes.clear();
}

bool Application::Initialize()
{
	if (!mIsInitialized)
	{
		// Factory
#ifdef _DEBUG
		{
			Microsoft::WRL::ComPtr<ID3D12Debug> pDebugController{};
			if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&pDebugController))))
				pDebugController->EnableDebugLayer();
		}
#endif
		ThrowIfFailed(CreateDXGIFactory(IID_PPV_ARGS(&mpDxgiFactory)));
		
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

		mIsInitialized = true;
	}
	return true;
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

Scene* Application::AddScene()
{
	mScenes.emplace_back(this);
	return &mScenes.back();
}

Window* Application::AddWindow(const std::wstring& title, Ion::Core::Rectangle<int> rectangle)
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

const Microsoft::WRL::ComPtr<IDXGIFactory>& Application::GetDxgiFactory()
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

Material* Application::AddMaterial(const std::string& name)
{
	auto ret{ mMaterials.try_emplace(name, this, name) };
	Material* pMaterial{ &((*(ret.first)).second) };
	pMaterial->Initialize();
	return pMaterial;
}
