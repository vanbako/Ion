#include "../Test/pch.h"
#include "../Core/MeshVC.h"
#include "../Core/ModelVC.h"
#include "../Core/Object.h"
#include <iostream>

using namespace Ion::Core;

static const std::chrono::microseconds gMainSleep{ 500 };

Object* AddFlower(Scene* pScene, Canvas* pCanvas);
Object* AddWizard(Scene* pScene, Canvas* pCanvas);

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
		pCube->GetModelC<TransformMC>()->SetPosition(DirectX::XMFLOAT4{ 20.f, 0.5f, 0.f, 0.f });

		Object* pFlower{ AddFlower(pScene, pCanvas) };
		Object* pWizard{ AddWizard(pScene, pCanvas) };

		Object* pCamera{ Factory::AddCamera(pScene) };
		TransformMC* pCameraTransform{ pCamera->GetModelC<TransformMC>() };
		pCameraTransform->SetPosition(DirectX::XMFLOAT4{ 0.f, 20.f, -100.f, 0.f });
		pCameraTransform->SetRotation(DirectX::XMFLOAT3{ 0.f, 0.f, 0.f }, AngleUnit::Degree);
		pCanvas->SetCamera(pCamera);

		pScene->Initialize();

		pCube->SetIsActive(true, true);
		pCamera->SetIsActive(true, true);
		pFlower->SetIsActive(true, true);
		pWizard->SetIsActive(true, true);

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

Object* AddFlower(Scene* pScene, Canvas* pCanvas)
{
	Object* pFlower{ pScene->AddObject(false) };
	TransformMC* pFlowerTransformMC{ pFlower->AddModelC<TransformMC>(false) };
	pFlowerTransformMC->SetPosition(DirectX::XMFLOAT4{ -15.f, 5.f, 0.f, 0.f });
	ModelVC* pFlowerModelVC{ pFlower->AddViewC<ModelVC>("Flower", "PosNormTanTex_A", false) };
	pFlowerModelVC->AddTexture(TextureType::Albedo, "Flower_Blue.png");
	pFlowerModelVC->AddCanvas(pCanvas);
	return pFlower;
}

Object* AddWizard(Scene* pScene, Canvas* pCanvas)
{
	Object* pWizard{ pScene->AddObject(false) };
	TransformMC* pWizardTransformMC{ pWizard->AddModelC<TransformMC>(false) };
	pWizardTransformMC->SetPosition(DirectX::XMFLOAT4{ 0.f, 0.f, 0.f, 0.f });
	pWizardTransformMC->SetRotation(DirectX::XMFLOAT3{ 90.f, 0.f, 0.f }, AngleUnit::Degree);
	ModelVC* pWizardModelVC{ pWizard->AddViewC<ModelVC>("Wizard", "PosNormTanTex_AN", false) };
	pWizardModelVC->AddTexture(TextureType::Albedo, "Wizard_Blue_A.png");
	pWizardModelVC->AddTexture(TextureType::Normal, "Wizard_Blue_N.png");
	pWizardModelVC->AddCanvas(pCanvas);
	return pWizard;
}