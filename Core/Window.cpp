#include "../Core/pch.h"
#include "../Core/Window.h"

using namespace Ion::Core;

Window::Window(Application* pApplication, const std::wstring title, Ion::Core::Rectangle<int> rectangle)
	: mpApplication{ pApplication }
	, mhWindow{}
{
	mhWindow = CreateWindow(
		L"IonEngineWindowClass",
		title.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rectangle.GetWidth(),
		rectangle.GetHeight(),
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

Canvas* Window::AddCanvas(Ion::Core::Rectangle<int> rectangle)
{
	mCanvases.emplace_back(this, rectangle);
	return &mCanvases.back();
}
