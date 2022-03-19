#pragma once

namespace Ion
{
	namespace Core
	{
		class Application;
		class Canvas;
		class ViewC;

		class Material
		{
		public:
			explicit Material(Core::Application* pApplication, const std::string& name);
			virtual ~Material() = default;
			Material(const Material& other) = delete;
			Material(Material&& other) noexcept = delete;
			Material& operator=(const Material& other) = delete;
			Material& operator=(Material&& other) noexcept = delete;

			virtual void Initialize();
			virtual void Render(Core::Canvas* pCanvas) = 0;
			virtual void ViewCUpdate(Core::Canvas* pCanvas, float delta) = 0;

			virtual void AddViewC(Core::Canvas* pCanvas, Core::ViewC* pViewC) = 0;
		protected:
			bool mIsInitialized;
			Core::Application* mpApplication;
			std::string mName;
		};
	}
}
