#pragma once

namespace Ion
{
	namespace Core
	{
		class Application;
		class Canvas;

		class Window final
		{
		public:
			explicit Window(Core::Application* pApplication, const std::wstring title, RECT rectangle);
			~Window();
			Window(const Window& other) = delete;
			Window(Window&& other) noexcept = delete;
			Window& operator=(const Window& other) = delete;
			Window& operator=(Window&& other) noexcept = delete;

			Core::Application* GetApplication();
			HWND& GetHandle();
			void AddCanvas(Core::Canvas* pCanvas);
			POINT GetCursorPosition();
		private:
			Core::Application* mpApplication;
			HWND mhWindow;
			std::vector<Core::Canvas*> mpCanvases;
		};
	}
}
