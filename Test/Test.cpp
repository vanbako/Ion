#include "../Test/pch.h"
#include "../Core/Application.h"
#include "../Core/Factory.h"
#include "../Core/MeshVC.h"
#include "../Core/ModelVC.h"
#include "../Core/TriangleVC.h"
#include "../Core/InstancedModelVC.h"
#include "../Core/Object.h"
#include <iostream>

using namespace Ion::Core;

Object* AddCamera(Scene* pScene, Canvas* pCanvas);
Object* AddCube(Scene* pScene, Canvas* pCanvas3D, Canvas* pCanvas2D);
Object* AddFlower(Scene* pScene, Canvas* pCanvas);
Object* AddWizard(Scene* pScene, Canvas* pCanvas);

int main()
{
	{
		RECT rectangle{ 0, 0, 1280, 720 };
		Application application{};
		Scene* pScene{ application.AddScene() };

		application.Initialize();

		// 3D
		Window* pWindow3D{ application.AddWindow(L"Main 3D Render Window", rectangle) };
		Canvas* pCanvas3D{ pWindow3D->AddCanvas(rectangle) };
		pScene->AddCanvas(pCanvas3D);
		pCanvas3D->Initialize();
		// 2D
		Window* pWindow2D{ application.AddWindow(L"Main 2D Render Window", rectangle) };
		Canvas* pCanvas2D{ pWindow2D->AddCanvas(rectangle) };
		pScene->AddCanvas(pCanvas2D);
		pCanvas2D->Initialize();

		Object* pCube{ AddCube(pScene, pCanvas3D, pCanvas2D) };
		Object* pFlower{ AddFlower(pScene, pCanvas3D) };
		Object* pWizard{ AddWizard(pScene, pCanvas3D) };
		Object* pCamera{ AddCamera(pScene, pCanvas3D) };

		pScene->Initialize();

		pCube->SetIsActive(true, true);
		pCamera->SetIsActive(true, true);
		pFlower->SetIsActive(true, true);
		pWizard->SetIsActive(true, true);

		pScene->SetIsActive(true);
#ifdef _DEBUG
		std::cout << "App is running" << std::endl;
#endif
		application.Run();

		pScene->SetIsActive(false);
#ifdef _DEBUG
		std::cout << "App is shutting down" << std::endl;
#endif
		pScene->SetIsEnd(true);
	}
#ifdef _DEBUG
	std::cout << "Done" << std::endl;
#endif
}

Object* AddCamera(Scene* pScene, Canvas* pCanvas)
{
	Object* pCamera{ Factory::AddCamera(pScene) };
	TransformMC* pCameraTransform{ pCamera->GetModelC<TransformMC>() };
	pCameraTransform->SetPosition(DirectX::XMFLOAT4{ 0.f, 20.f, -100.f, 0.f });
	pCameraTransform->SetRotation(DirectX::XMFLOAT3{ 0.f, 0.f, 0.f }, AngleUnit::Degree);
	pCanvas->SetCamera(pCamera);
	return pCamera;
}

Object* AddCube(Scene* pScene, Canvas* pCanvas3D, Canvas* pCanvas2D)
{
	Object* pCube{ Factory::AddCube(pScene) };
	pCube->GetViewC<MeshVC>()->AddCanvas(pCanvas3D);
	pCube->GetModelC<TransformMC>()->SetPosition(DirectX::XMFLOAT4{ 20.f, 0.5f, 0.f, 0.f });
	pCube->GetViewC<TriangleVC>()->AddCanvas(pCanvas3D);
	pCube->GetViewC<TriangleVC>()->AddCanvas(pCanvas2D);
	return pCube;
}

Object* AddFlower(Scene* pScene, Canvas* pCanvas)
{
	Object* pFlower{ pScene->AddObject(false) };
	TransformMC* pTransformMC{ pFlower->AddModelC<TransformMC>(false) };
	(pTransformMC);
	//pTransformMC->SetPosition(DirectX::XMFLOAT4{ -15.f, 5.f, 0.f, 0.f });
	InstancedModelVC* pFlowerModelVC{ pFlower->AddViewC<InstancedModelVC>("Flower", "PosNormTanTex_iA", false, Winding::CCW) };
	pFlowerModelVC->AddTexture(TextureType::Albedo, "Flower_Blue.png");
	pFlowerModelVC->AddCanvas(pCanvas);
	std::vector<TransformMC> transforms{};
	for (size_t i{ 0 }; i < 100; ++i)
	{
		transforms.emplace_back(true, pFlower);
		transforms.back().SetPosition(DirectX::XMFLOAT4{ 10.f * float(i / 10 + 1), 0.f, 10.f * float(i % 10 + 1), 0.f });
	}
	pFlowerModelVC->AddInstances(transforms);
	return pFlower;
}

Object* AddWizard(Scene* pScene, Canvas* pCanvas)
{
	Object* pWizard{ pScene->AddObject(false) };
	TransformMC* pTransformMC{ pWizard->AddModelC<TransformMC>(false) };
	(pTransformMC);
	//pTransformMC->SetPosition(DirectX::XMFLOAT4{ 0.25f, 0.f, 0.f, 0.f });
	//pTransformMC->SetRotation(DirectX::XMFLOAT3{ 90.f, 0.f, 0.f }, AngleUnit::Degree);
	InstancedModelVC* pWizardModelVC{ pWizard->AddViewC<InstancedModelVC>("Wizard", "PosNormTanTex_iAN", false, Winding::CCW) };
	pWizardModelVC->AddTexture(TextureType::Albedo, "Wizard_Blue_A.png");
	pWizardModelVC->AddTexture(TextureType::Normal, "Wizard_Blue_N.png");
	pWizardModelVC->AddCanvas(pCanvas);
	//TransformMC transformMC{ true, pWizard };
	//transformMC.SetPosition(DirectX::XMFLOAT4{ 10.f, 0.f, 0.f, 0.f });
	//transformMC.SetRotation(DirectX::XMFLOAT3{ 90.f, 0.f, 0.f }, AngleUnit::Degree);
	//pWizardModelVC->AddInstance(transformMC);
	//transformMC.SetPosition(DirectX::XMFLOAT4{ -10.f, 0.f, 0.f, 0.f });
	//transformMC.SetRotation(DirectX::XMFLOAT3{ 90.f, 0.f, 0.f }, AngleUnit::Degree);
	//pWizardModelVC->AddInstance(transformMC);
	std::vector<TransformMC> transforms{};
	for (size_t i{ 0 }; i < 20; ++i)
	{
		transforms.emplace_back(true, pWizard);
		transforms.back().SetPosition(DirectX::XMFLOAT4{ 40.f * float(int(i) - 10), 0.f, 0.f, 0.f });
		transforms.back().SetRotation(DirectX::XMFLOAT3{ 90.f, 0.f, 0.f }, AngleUnit::Degree);
	}
	pWizardModelVC->AddInstances(transforms);
	return pWizard;
}
