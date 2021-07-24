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
#include "include/FmodAudio.h"
#include <iostream>

using namespace Ion;

Core::Object* AddCamera(Core::Scene* pScene, Core::Canvas* pCanvas);
Core::Object* AddCube(Core::Scene* pScene, Core::Canvas* pCanvas1, Core::Canvas* pCanvas2);
Core::Object* AddFlower(Core::Scene* pScene, Core::Canvas* pCanvas);
Core::Object* AddWizard(Core::Scene* pScene, Core::Canvas* pCanvas1, Core::Canvas* pCanvas2);
Core::Object* AddRoyalHighness(Core::Scene* pScene, Core::Canvas* pCanvas);

int main()
{
	{
		RECT rectangle{ 0, 0, 1280, 720 };
		Core::Application application{};

		// Services
		Core::ServiceLocator& serviceLocator{ application.GetServiceLocator() };
		Core::FileLogger fileLogger{ "Test.log" };
		serviceLocator.RegisterLoggerService(&fileLogger);
		FmodAudio fmodAudio{};
		serviceLocator.RegisterAudioService(&fmodAudio);

		Core::Scene* pScene{ application.AddScene() };

		application.Initialize();
		// Window 2
		Core::Window* pWindow2{ application.AddWindow(L"Render Window 2", rectangle) };
		Core::Canvas* pCanvas2{ pWindow2->AddCanvas(rectangle) };
		pScene->AddCanvas(pCanvas2);
		pCanvas2->Initialize();
		// Window 1
		Core::Window* pWindow1{ application.AddWindow(L"Render Window 1", rectangle) };
		Core::Canvas* pCanvas1{ pWindow1->AddCanvas(rectangle) };
		pScene->AddCanvas(pCanvas1);
		pCanvas1->Initialize();

		Core::Object* pCube{ AddCube(pScene, pCanvas1, pCanvas2) };
		Core::Object* pFlower{ AddFlower(pScene, pCanvas1) };
		Core::Object* pWizard{ AddWizard(pScene, pCanvas1, pCanvas2) };
		Core::Object* pRoyalHighness{ AddRoyalHighness(pScene, pCanvas1) };
		Core::Object* pCamera1{ AddCamera(pScene, pCanvas1) };
		Core::Object* pCamera2{ AddCamera(pScene, pCanvas2) };

		pScene->Initialize();

		Core::Factory::CreatePhysicsPlane(application.GetPhysics(), pScene->GetPxScene());

		pCube->SetIsActive(true, true);
		pCamera1->SetIsActive(true, true);
		pCamera2->SetIsActive(true, true);
		pFlower->SetIsActive(true, true);
		pWizard->SetIsActive(true, true);
		pRoyalHighness->SetIsActive(true, true);

		Core::AnimatedModelVC* pRoyalHighnessModelVC{ pRoyalHighness->GetViewC<Core::AnimatedModelVC>() };
		pRoyalHighnessModelVC->SetAnimation(0);
		pRoyalHighnessModelVC->SetIsAnimating(true);

		pScene->SetIsActive(true);
#ifdef _DEBUG
		std::cout << "App is running" << std::endl;
#endif
		int
			startSound{ serviceLocator.GetAudio()->AddSound("bbc_world-war-_07017169.mp3", false) },
			stopSound{ serviceLocator.GetAudio()->AddSound("bbc_peugeot-20_07055235.mp3", false) };
		serviceLocator.GetAudio()->PlaySound(startSound);
		application.Run();
		serviceLocator.GetAudio()->PlaySound(stopSound);
		std::this_thread::sleep_for(std::chrono::seconds{ 4 });

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

Core::Object* AddCamera(Core::Scene* pScene, Core::Canvas* pCanvas)
{
	Core::Object* pCamera{ Core::Factory::AddCamera(pScene) };
	Core::TransformMC* pCameraTransform{ pCamera->GetModelC<Core::TransformMC>() };
	pCameraTransform->SetPosition(DirectX::XMFLOAT4{ 0.f, 20.f, -150.f, 0.f });
	pCameraTransform->SetRotation(DirectX::XMFLOAT3{ 0.f, 0.f, 0.f }, Core::AngleUnit::Degree);
	pCanvas->SetCamera(pCamera);
	return pCamera;
}

Core::Object* AddCube(Core::Scene* pScene, Core::Canvas* pCanvas1, Core::Canvas* pCanvas2)
{
	Core::Object* pCube{ Core::Factory::AddCube(pScene) };
	pCube->GetViewC<Core::MeshVC>()->AddCanvas(pCanvas1);
	Core::TransformMC* pTransformMC{ pCube->GetModelC<Core::TransformMC>() };
	pTransformMC->SetPosition(DirectX::XMFLOAT4{ 20.f, 40.0f, 0.f, 0.f });

	physx::PxPhysics* pPxPhysics{ &pScene->GetPxScene()->getPhysics() };
	physx::PxRigidDynamic* pPxRigidDynamic{ pPxPhysics->createRigidDynamic(physx::PxTransform{ physx::PxVec3{ 20.f, 40.f, 0.f } }) };
	physx::PxMaterial* pPxMaterial{ pPxPhysics->createMaterial(0.5f, 0.5f, 0.1f) };
	physx::PxShape* pShape{ pPxPhysics->createShape(physx::PxBoxGeometry{ 0.5f, 0.5f, 0.5f }, *pPxMaterial) };
	pPxRigidDynamic->attachShape(*pShape);
	pScene->GetPxScene()->addActor(*pPxRigidDynamic);
	pPxRigidDynamic->wakeUp();

	pTransformMC->SetPxRigidActor(pPxRigidDynamic);
	pCube->GetViewC<Core::TriangleVC>()->AddCanvas(pCanvas1);
	pCube->GetViewC<Core::TriangleVC>()->AddCanvas(pCanvas2);
	return pCube;
}

Core::Object* AddFlower(Core::Scene* pScene, Core::Canvas* pCanvas)
{
	Core::Object* pFlower{ pScene->AddObject(false) };
	Core::TransformMC* pTransformMC{ pFlower->AddModelC<Core::TransformMC>(false) };
	(pTransformMC);
	//pTransformMC->SetPosition(DirectX::XMFLOAT4{ -15.f, 5.f, 0.f, 0.f });
	Core::InstancedModelVC* pFlowerModelVC{ pFlower->AddViewC<Core::InstancedModelVC>("Flower/Flower", "ovm", "PosNormTanTex_iA", false, Core::Winding::CCW)};
	pFlowerModelVC->AddTexture(Core::TextureType::Albedo, "Flower/Flower_Blue.png");
	pFlowerModelVC->AddCanvas(pCanvas);
	std::vector<Core::TransformMC> transforms{};
	for (size_t i{ 0 }; i < 10000; ++i)
	{
		transforms.emplace_back(true, pFlower);
		transforms.back().SetPosition(DirectX::XMFLOAT4{ 10.f * float(i / 100 + 1), 0.f, 10.f * float(i % 100 + 1), 0.f });
	}
	pFlowerModelVC->AddInstances(transforms);
	return pFlower;
}

Core::Object* AddWizard(Core::Scene* pScene, Core::Canvas* pCanvas1, Core::Canvas* pCanvas2)
{
	Core::Object* pWizard{ pScene->AddObject(false) };
	Core::TransformMC* pTransformMC{ pWizard->AddModelC<Core::TransformMC>(false) };
	(pTransformMC);
	//pTransformMC->SetPosition(DirectX::XMFLOAT4{ 0.25f, 0.f, 0.f, 0.f });
	//pTransformMC->SetRotation(DirectX::XMFLOAT3{ 90.f, 0.f, 0.f }, AngleUnit::Degree);
	Core::InstancedModelVC* pWizardModelVC{ pWizard->AddViewC<Core::InstancedModelVC>("Wizard/Wizard", "ovm", "PosNormTanTex_iAN", false, Core::Winding::CCW, Core::CoordSystem::RightHanded) };
	pWizardModelVC->AddTexture(Core::TextureType::Albedo, "Wizard/Wizard_Blue_A.png");
	pWizardModelVC->AddTexture(Core::TextureType::Normal, "Wizard/Wizard_Blue_N.png");
	pWizardModelVC->AddCanvas(pCanvas1);
	pWizardModelVC->AddCanvas(pCanvas2);
	//TransformMC transformMC{ true, pWizard };
	//transformMC.SetPosition(DirectX::XMFLOAT4{ 10.f, 0.f, 0.f, 0.f });
	//transformMC.SetRotation(DirectX::XMFLOAT3{ 90.f, 0.f, 0.f }, AngleUnit::Degree);
	//pWizardModelVC->AddInstance(transformMC);
	//transformMC.SetPosition(DirectX::XMFLOAT4{ -10.f, 0.f, 0.f, 0.f });
	//transformMC.SetRotation(DirectX::XMFLOAT3{ 90.f, 0.f, 0.f }, AngleUnit::Degree);
	//pWizardModelVC->AddInstance(transformMC);
	std::vector<Core::TransformMC> transforms{};
	for (size_t i{ 0 }; i < 10; ++i)
	{
		transforms.emplace_back(true, pWizard);
		transforms.back().SetPosition(DirectX::XMFLOAT4{ 40.f * float(int(i) - 5), 0.f, 0.f, 0.f });
		//transforms.back().SetRotation(DirectX::XMFLOAT3{ 90.f, 0.f, 0.f }, AngleUnit::Degree);
	}
	pWizardModelVC->AddInstances(transforms);
	return pWizard;
}

Core::Object* AddRoyalHighness(Core::Scene* pScene, Core::Canvas* pCanvas)
{
	Core::Object* pRoyalHighness{ pScene->AddObject(false) };
	Core::TransformMC* pTransformMC{ pRoyalHighness->AddModelC<Core::TransformMC>(false) };
	pTransformMC->SetPosition(DirectX::XMFLOAT4{ 0.f, 0.f, -25.f, 0.f });
	//pTransformMC->SetScale(DirectX::XMFLOAT4{ 0.5f, 0.5f, 0.5f, 0.f });
	//pTransformMC->SetRotation(DirectX::XMFLOAT3{ 90.f, 0.f, 0.f }, Core::AngleUnit::Degree);
	Core::AnimatedModelVC* pRoyalHighnessModelVC{ pRoyalHighness->AddViewC<Core::AnimatedModelVC>("RoyalHighness/RoyalHighness1", "ovm", "PosNormTanTex_aAN", false, Core::Winding::CCW, Core::CoordSystem::RightHanded) };
	//ModelVC* pRoyalHighnessModelVC{ pRoyalHighness->AddViewC<ModelVC>("RoyalHighness/RoyalHighness1", "PosNormTanTex_AN", false, Winding::CCW, CoordSystem::RightHanded) };
	pRoyalHighnessModelVC->AddTexture(Core::TextureType::Albedo, "RoyalHighness/RoyalHighness_Red_A.png");
	pRoyalHighnessModelVC->AddTexture(Core::TextureType::Normal, "RoyalHighness/RoyalHighness_Red_N.png");
	pRoyalHighnessModelVC->AddCanvas(pCanvas);
	return pRoyalHighness;
}
