#include "../Test/pch.h"
#include "../Core/MeshVC.h"
#include "../Core/Object.h"
#include <iostream>

using namespace Ion::Core;

int main()
{
	{
		Ion::Core::Rectangle<int> rectangle{ 0, 0, 1280, 720 };
		Application application{};
		Scene* pScene{ application.AddScene() };

		application.Initialize();

		Window* pWindow{ application.AddWindow(L"Main Render Window", rectangle) };
		Canvas* pCanvas{ pWindow->AddCanvas(rectangle) };
		pScene->AddCanvas(pCanvas);
		pCanvas->Initialize();

		Object* pCube{ Factory::AddCube(pScene) };
		pCube->GetViewC<MeshVC>()->AddCanvas(pCanvas);

		Object* pCamera{ Factory::AddCamera(pScene) };
		TransformMC* pCameraTransform{ pCamera->GetModelC<TransformMC>() };
		pCameraTransform->SetPosition(DirectX::XMFLOAT3{ 0.f, 0.f, -10.f });
		pCameraTransform->SetRotation(DirectX::XMFLOAT3{ 0.f, 0.f, 0.f }, AngleUnit::Degree);
		pCanvas->SetCamera(pCamera);

		pScene->Initialize();

		Factory::SetCameraActive(pCamera);
		Factory::SetCubeActive(pCube);
		pScene->SetIsActive(true);

		std::cout << "App is running" << std::endl;
		//std::string str{};
		//std::cin >> str;
		std::chrono::microseconds sleep{ 1000 };
		MSG msg{ 0 };
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
				std::this_thread::sleep_for(sleep);
		}
		pScene->SetIsActive(false);
		std::cout << "App is shutting down" << std::endl;
	}
	std::cout << "Done" << std::endl;
	//std::string str{};
	//std::cin >> str;
}
