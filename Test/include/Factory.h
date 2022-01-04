#pragma once

namespace Ion
{
	namespace Core
	{
		class Application;
		class Scene;
		class Object;
		class Canvas;
	}
	namespace Test
	{
		class Factory final
		{
		public:
			explicit Factory() = delete;
			~Factory() = delete;
			Factory(const Factory& other) = delete;
			Factory(Factory&& other) noexcept = delete;
			Factory& operator=(const Factory& other) = delete;
			Factory& operator=(Factory&& other) noexcept = delete;

			static Core::Object* AddCamera(Core::Scene* pScene, Core::Canvas* pCanvas);
			static Core::Object* AddTerrain(Core::Scene* pScene, Core::Canvas* pCanvas);
			static Core::Object* AddCube(Core::Scene* pScene, Core::Canvas* pCanvas);
			static Core::Object* AddFlowerOne(Core::Scene* pScene, Core::Canvas* pCanvas);
			static Core::Object* AddFlowerTwo(Core::Scene* pScene, Core::Canvas* pCanvas);
			static Core::Object* AddWizard(Core::Scene* pScene, Core::Canvas* pCanvas);
			static Core::Object* AddRoyalHighness(Core::Scene* pScene, Core::Canvas* pCanvas);
			static Core::Scene* AddSceneOne(Core::Application* pApplication, Core::Canvas* pCanvas);
			static Core::Scene* AddSceneTwo(Core::Application* pApplication, Core::Canvas* pCanvas);
			static Core::Scene* AddSceneZero(Core::Application* pApplication);
		};
	}
}
