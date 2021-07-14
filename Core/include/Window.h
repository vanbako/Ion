#pragma once
#include "Canvas.h"

namespace Ion
{
	namespace Core
	{
		class Application;

		class Window final
		{
		public:
			explicit Window(Application* pApplication, const std::wstring title, RECT rectangle);
			~Window() = default;
			Window(const Window& other) = delete;
			Window(Window&& other) noexcept = delete;
			Window& operator=(const Window& other) = delete;
			Window& operator=(Window&& other) noexcept = delete;

			Application* GetApplication();
			HWND& GetHandle();
			Canvas* AddCanvas(RECT rectangle);
		private:
			Application* mpApplication;
			HWND mhWindow;
			std::list<Canvas> mCanvases;
		};
	}
}
