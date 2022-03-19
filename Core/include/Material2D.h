#pragma once
#include "Material.h"

namespace Ion
{
	namespace Core
	{
		class Application;
		class Canvas;
		class ViewC;

		class Material2D final
			: public Material
		{
		public:
			explicit Material2D(Core::Application* pApplication, const std::string& name);
			~Material2D() = default;
			Material2D(const Material2D& other) = delete;
			Material2D(Material2D&& other) noexcept = delete;
			Material2D& operator=(const Material2D& other) = delete;
			Material2D& operator=(Material2D&& other) noexcept = delete;

			void Initialize() override;
			void Render(Core::Canvas* pCanvas) override;
			void ViewCUpdate(Core::Canvas* pCanvas, float delta) override { (pCanvas); (delta); };

			void AddViewC(Core::Canvas* pCanvas, Core::ViewC* pViewC) override;
		private:
			std::unordered_map<Core::Canvas*, std::vector<Core::ViewC*>> mpCanvasViewCs;
		};
	}
}
