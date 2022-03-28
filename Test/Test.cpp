#include "pch.h"

//#define TEST_SOUND
#define SCENE0
#define SCENE1
#define SCENE2

#include "../Core/include/Application.h"
#include "../Core/include/FileLogger.h"
#include "../Test/include/Factory.h"
#ifdef TEST_SOUND
#include "../Test/include/FmodAudio.h"
#endif

using namespace Ion;

//int main()
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	(hInstance);
	(hPrevInstance);
	(lpCmdLine);
	(nShowCmd);
#ifdef _DEBUG
	std::cout << "Start" << std::endl;
#endif
	{
		RECT rectangle{ 0, 0, 1280, 720 };
		Core::Application application{};

		Core::ServiceLocator& serviceLocator{ application.GetServiceLocator() };
#if !defined ION_LOGGER && !defined TEST_SOUND
		(serviceLocator);
#endif
#ifdef ION_LOGGER
		Core::FileLogger fileLogger{ "Test.log" };
		serviceLocator.RegisterLoggerService(&fileLogger);
#endif
#ifdef TEST_SOUND
		FmodAudio fmodAudio{};
		serviceLocator.RegisterAudioService(&fmodAudio);
#endif
		application.Initialize();
#ifdef SCENE0
		Core::Scene* pScene0{ Test::Factory::AddSceneZero(&application) };
		pScene0->SetIsActive(true);
#endif
#ifdef SCENE1
		Core::Scene* pScene1{ nullptr };
		{
			Core::Window* pWindow{ application.AddWindow(L"Render Window 1", rectangle) };
			Core::Canvas* pCanvas{ application.AddCanvas(rectangle) };
			pWindow->AddCanvas(pCanvas);
			pCanvas->Initialize();
			pScene1 = Test::Factory::AddSceneOne(&application, pCanvas);
		}
		pScene1->SetIsActive(true);
#endif
#ifdef SCENE2
		Core::Scene* pScene2{ nullptr };
		{
			Core::Window* pWindow{ application.AddWindow(L"Render Window 2", rectangle) };
			Core::Canvas* pCanvas{ application.AddCanvas(rectangle) };
			pWindow->AddCanvas(pCanvas);
			pCanvas->Initialize();
			pScene2 = Test::Factory::AddSceneTwo(&application, pCanvas);
		}
		pScene2->SetIsActive(true);
#endif

#ifdef TEST_SOUND
		int
			startSound{ serviceLocator.GetAudio()->AddSound("bbc_world-war-_07017169.mp3", false) },
			stopSound{ serviceLocator.GetAudio()->AddSound("bbc_peugeot-20_07055235.mp3", false) };
		serviceLocator.GetAudio()->PlaySound(startSound);
#endif
#ifdef ION_LOGGER
		application.GetServiceLocator().GetLogger()->Message(nullptr, Core::MsgType::Info, "Application before run");
#endif
		application.Run();
#ifdef ION_LOGGER
		application.GetServiceLocator().GetLogger()->Message(nullptr, Core::MsgType::Info, "Application after run");
#endif
#ifdef TEST_SOUND
		serviceLocator.GetAudio()->PlaySound(stopSound);
		std::this_thread::sleep_for(std::chrono::seconds{ 4 });
#endif
#ifdef SCENE0
		pScene0->SetIsActive(false);
		pScene0->SetIsEnd(true);
#endif
#ifdef SCENE1
		pScene1->SetIsActive(false);
		pScene1->SetIsEnd(true);
#endif
#ifdef SCENE2
		pScene2->SetIsActive(false);
		pScene2->SetIsEnd(true);
#endif
#ifdef ION_LOGGER
		application.GetServiceLocator().GetLogger()->Message(nullptr, Core::MsgType::Info, "Application shutdown");
#endif
	}
#ifdef _DEBUG
	std::cout << "Done" << std::endl;
#endif
}
