#pragma once

namespace Ion
{
	namespace Core
	{
		class Application;
		class Canvas;
		class ViewC;

		class Material2D final
		{
		public:
			explicit Material2D(Application* pApplication, const std::string& name);
			~Material2D() = default;
			Material2D(const Material2D& other) = delete;
			Material2D(Material2D&& other) noexcept = delete;
			Material2D& operator=(const Material2D& other) = delete;
			Material2D& operator=(Material2D&& other) noexcept = delete;

			void Initialize();
			void Render(Canvas* pCanvas);

			void AddViewC(Canvas* pCanvas, ViewC* pViewC);
		private:
			bool mIsInitialized;
			Application* mpApplication;
			std::string mName;
			std::map<Canvas*,std::vector<ViewC*>> mpCanvasViewCs;
		};
	}
}
