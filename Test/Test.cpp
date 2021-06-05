#include "../Test/pch.h"
#include "../Core/MeshVC.h"
#include "../Core/Object.h"
#include <iostream>

using namespace Ion::Core;

static const std::chrono::microseconds gMainSleep{ 500 };

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

		MSG msg{};
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
				std::this_thread::sleep_for(gMainSleep);
		}
		pScene->SetIsActive(false);
		std::cout << "App is shutting down" << std::endl;
		pScene->SetIsEnd(true);
	}
	std::cout << "Done" << std::endl;
}
