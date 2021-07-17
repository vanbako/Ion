#include "pch.h"
#include "Application.h"
#include "Factory.h"
#include "MeshVC.h"
#include "ModelVC.h"
#include "TriangleVC.h"
#include "InstancedModelVC.h"
#include "AnimatedModelVC.h"
#include "Object.h"
#include "FileLogger.h"
#include <iostream>

using namespace Ion::Core;

Object* AddCamera(Scene* pScene, Canvas* pCanvas);
Object* AddCube(Scene* pScene, Canvas* pCanvas1, Canvas* pCanvas2);
Object* AddFlower(Scene* pScene, Canvas* pCanvas);
Object* AddWizard(Scene* pScene, Canvas* pCanvas1, Canvas* pCanvas2);
Object* AddRoyalHighness(Scene* pScene, Canvas* pCanvas);

int main()
{
	{
		RECT rectangle{ 0, 0, 1280, 720 };
		Application application{};

		FileLogger fileLogger{ "Test.log" };
		ServiceLocator& serviceLocator{ application.GetServiceLocator() };
		serviceLocator.RegisterLoggerService(&fileLogger);

		Scene* pScene{ application.AddScene() };

		application.Initialize();
		// Window 2
		Window* pWindow2{ application.AddWindow(L"Render Window 2", rectangle) };
		Canvas* pCanvas2{ pWindow2->AddCanvas(rectangle) };
		pScene->AddCanvas(pCanvas2);
		pCanvas2->Initialize();
		// Window 1
		Window* pWindow1{ application.AddWindow(L"Render Window 1", rectangle) };
		Canvas* pCanvas1{ pWindow1->AddCanvas(rectangle) };
		pScene->AddCanvas(pCanvas1);
		pCanvas1->Initialize();

		Object* pCube{ AddCube(pScene, pCanvas1, pCanvas2) };
		Object* pFlower{ AddFlower(pScene, pCanvas1) };
		Object* pWizard{ AddWizard(pScene, pCanvas1, pCanvas2) };
		Object* pRoyalHighness{ AddRoyalHighness(pScene, pCanvas1) };
		Object* pCamera1{ AddCamera(pScene, pCanvas1) };
		Object* pCamera2{ AddCamera(pScene, pCanvas2) };

		pScene->Initialize();

		Factory::CreatePhysicsPlane(application.GetPhysics(), pScene->GetPxScene());

		pCube->SetIsActive(true, true);
		pCamera1->SetIsActive(true, true);
		pCamera2->SetIsActive(true, true);
		pFlower->SetIsActive(true, true);
		pWizard->SetIsActive(true, true);
		pRoyalHighness->SetIsActive(true, true);

		AnimatedModelVC* pRoyalHighnessModelVC{ pRoyalHighness->GetViewC<AnimatedModelVC>() };
		pRoyalHighnessModelVC->SetAnimation(0);
		pRoyalHighnessModelVC->SetIsAnimating(true);

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
	pCameraTransform->SetPosition(DirectX::XMFLOAT4{ 0.f, 20.f, -150.f, 0.f });
	pCameraTransform->SetRotation(DirectX::XMFLOAT3{ 0.f, 0.f, 0.f }, AngleUnit::Degree);
	pCanvas->SetCamera(pCamera);
	return pCamera;
}

Object* AddCube(Scene* pScene, Canvas* pCanvas1, Canvas* pCanvas2)
{
	Object* pCube{ Factory::AddCube(pScene) };
	pCube->GetViewC<MeshVC>()->AddCanvas(pCanvas1);
	TransformMC* pTransformMC{ pCube->GetModelC<TransformMC>() };
	pTransformMC->SetPosition(DirectX::XMFLOAT4{ 20.f, 40.0f, 0.f, 0.f });

	physx::PxPhysics* pPxPhysics{ &pScene->GetPxScene()->getPhysics() };
	physx::PxRigidDynamic* pPxRigidDynamic{ pPxPhysics->createRigidDynamic(physx::PxTransform{ physx::PxVec3{ 20.f, 40.f, 0.f } }) };
	physx::PxMaterial* pPxMaterial{ pPxPhysics->createMaterial(0.5f, 0.5f, 0.1f) };
	physx::PxShape* pShape{ pPxPhysics->createShape(physx::PxBoxGeometry{ 0.5f, 0.5f, 0.5f }, *pPxMaterial) };
	pPxRigidDynamic->attachShape(*pShape);
	pScene->GetPxScene()->addActor(*pPxRigidDynamic);
	pPxRigidDynamic->wakeUp();

	pTransformMC->SetPxRigidActor(pPxRigidDynamic);
	pCube->GetViewC<TriangleVC>()->AddCanvas(pCanvas1);
	pCube->GetViewC<TriangleVC>()->AddCanvas(pCanvas2);
	return pCube;
}

Object* AddFlower(Scene* pScene, Canvas* pCanvas)
{
	Object* pFlower{ pScene->AddObject(false) };
	TransformMC* pTransformMC{ pFlower->AddModelC<TransformMC>(false) };
	(pTransformMC);
	//pTransformMC->SetPosition(DirectX::XMFLOAT4{ -15.f, 5.f, 0.f, 0.f });
	InstancedModelVC* pFlowerModelVC{ pFlower->AddViewC<InstancedModelVC>("Flower/Flower", "PosNormTanTex_iA", false, Winding::CCW) };
	pFlowerModelVC->AddTexture(TextureType::Albedo, "Flower/Flower_Blue.png");
	pFlowerModelVC->AddCanvas(pCanvas);
	std::vector<TransformMC> transforms{};
	for (size_t i{ 0 }; i < 10000; ++i)
	{
		transforms.emplace_back(true, pFlower);
		transforms.back().SetPosition(DirectX::XMFLOAT4{ 10.f * float(i / 100 + 1), 0.f, 10.f * float(i % 100 + 1), 0.f });
	}
	pFlowerModelVC->AddInstances(transforms);
	return pFlower;
}

Object* AddWizard(Scene* pScene, Canvas* pCanvas1, Canvas* pCanvas2)
{
	Object* pWizard{ pScene->AddObject(false) };
	TransformMC* pTransformMC{ pWizard->AddModelC<TransformMC>(false) };
	(pTransformMC);
	//pTransformMC->SetPosition(DirectX::XMFLOAT4{ 0.25f, 0.f, 0.f, 0.f });
	//pTransformMC->SetRotation(DirectX::XMFLOAT3{ 90.f, 0.f, 0.f }, AngleUnit::Degree);
	InstancedModelVC* pWizardModelVC{ pWizard->AddViewC<InstancedModelVC>("Wizard/Wizard", "PosNormTanTex_iAN", false, Winding::CCW, CoordSystem::RightHanded) };
	pWizardModelVC->AddTexture(TextureType::Albedo, "Wizard/Wizard_Blue_A.png");
	pWizardModelVC->AddTexture(TextureType::Normal, "Wizard/Wizard_Blue_N.png");
	pWizardModelVC->AddCanvas(pCanvas1);
	pWizardModelVC->AddCanvas(pCanvas2);
	//TransformMC transformMC{ true, pWizard };
	//transformMC.SetPosition(DirectX::XMFLOAT4{ 10.f, 0.f, 0.f, 0.f });
	//transformMC.SetRotation(DirectX::XMFLOAT3{ 90.f, 0.f, 0.f }, AngleUnit::Degree);
	//pWizardModelVC->AddInstance(transformMC);
	//transformMC.SetPosition(DirectX::XMFLOAT4{ -10.f, 0.f, 0.f, 0.f });
	//transformMC.SetRotation(DirectX::XMFLOAT3{ 90.f, 0.f, 0.f }, AngleUnit::Degree);
	//pWizardModelVC->AddInstance(transformMC);
	std::vector<TransformMC> transforms{};
	for (size_t i{ 0 }; i < 10; ++i)
	{
		transforms.emplace_back(true, pWizard);
		transforms.back().SetPosition(DirectX::XMFLOAT4{ 40.f * float(int(i) - 5), 0.f, 0.f, 0.f });
		//transforms.back().SetRotation(DirectX::XMFLOAT3{ 90.f, 0.f, 0.f }, AngleUnit::Degree);
	}
	pWizardModelVC->AddInstances(transforms);
	return pWizard;
}

Object* AddRoyalHighness(Scene* pScene, Canvas* pCanvas)
{
	Object* pRoyalHighness{ pScene->AddObject(false) };
	TransformMC* pTransformMC{ pRoyalHighness->AddModelC<TransformMC>(false) };
	pTransformMC->SetPosition(DirectX::XMFLOAT4{ 0.f, 0.f, -25.f, 0.f });
	//pTransformMC->SetScale(DirectX::XMFLOAT4{ 0.5f, 0.5f, 0.5f, 0.f });
	//pTransformMC->SetRotation(DirectX::XMFLOAT3{ 90.f, 0.f, 0.f }, AngleUnit::Degree);
	AnimatedModelVC* pRoyalHighnessModelVC{ pRoyalHighness->AddViewC<AnimatedModelVC>("RoyalHighness/RoyalHighness1", "PosNormTanTex_aAN", false, Winding::CCW, CoordSystem::RightHanded) };
	//ModelVC* pRoyalHighnessModelVC{ pRoyalHighness->AddViewC<ModelVC>("RoyalHighness/RoyalHighness1", "PosNormTanTex_AN", false, Winding::CCW, CoordSystem::RightHanded) };
	pRoyalHighnessModelVC->AddTexture(TextureType::Albedo, "RoyalHighness/RoyalHighness_Red_A.png");
	pRoyalHighnessModelVC->AddTexture(TextureType::Normal, "RoyalHighness/RoyalHighness_Red_N.png");
	pRoyalHighnessModelVC->AddCanvas(pCanvas);
	return pRoyalHighness;
}
