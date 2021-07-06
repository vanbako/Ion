#include "../Core/pch.h"
#include "../Core/Window.h"

using namespace Ion::Core;

Window::Window(Application* pApplication, const std::wstring title, RECT rectangle)
	: mpApplication{ pApplication }
	, mhWindow{}
	, mCanvases{}
{
	mhWindow = CreateWindow(
		L"IonEngineWindowClass",
		title.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rectangle.right - rectangle.left,
		rectangle.bottom - rectangle.top,
		HWND{},
		HMENU{},
		GetModuleHandle(nullptr),
		pApplication
	);
	ShowWindow(mhWindow, SW_SHOWNORMAL);
	UpdateWindow(mhWindow);
}

Application* Window::GetApplication()
{
	return mpApplication;
}

HWND& Window::GetHandle()
{
	return mhWindow;
}

Canvas* Window::AddCanvas(RECT rectangle)
{
	mCanvases.emplace_back(this, rectangle);
	return &mCanvases.back();
}
