#include "pch.h"
#include "Application.h"
#include "Factory.h"
#include "MeshVC.h"
#include "ModelVC.h"
#include "InstancedTransformMC.h"
#include "TriangleVC.h"
#include "TextVC.h"
#include "InstancedModelVC.h"
#include "AnimatedModelVC.h"
#include "InstancedAnimatedModelVC.h"
#include "MoveObjectRMC.h"
#include "ControlRMC.h"
#include "SteeringRMC.h"
#include "InputCC.h"
#include "BehaviourCC.h"
#include "ControllerST.h"
#include "Object.h"
#include "FileLogger.h"
#include "include/FmodAudio.h"
#include <iostream>

//#define SOUND

using namespace Ion;

Core::Object* AddCamera(Core::Scene* pScene, Core::Canvas* pCanvas);
Core::Object* AddCube(Core::Scene* pScene, Core::Canvas* pCanvas1, Core::Canvas* pCanvas2);
Core::Object* AddFlower(Core::Scene* pScene, Core::Canvas* pCanvas);
Core::Object* AddWizard(Core::Scene* pScene, Core::Canvas* pCanvas1, Core::Canvas* pCanvas2);
Core::Object* AddRoyalHighness(Core::Scene* pScene, Core::Canvas* pCanvas);
Core::Object* AddControl(Core::Scene* pScene, Core::Canvas* pCanvas);

int main()
{
#ifdef _DEBUG
	std::cout << "Start" << std::endl;
#endif
	{
		RECT rectangle{ 0, 0, 1280, 720 };
		Core::Application application{};

		// Services
		Core::ServiceLocator& serviceLocator{ application.GetServiceLocator() };
		Core::FileLogger fileLogger{ "Test.log" };
		serviceLocator.RegisterLoggerService(&fileLogger);
#ifdef SOUND
		FmodAudio fmodAudio{};
		serviceLocator.RegisterAudioService(&fmodAudio);
#endif

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
		Core::Object* pControl{ AddControl(pScene, pCanvas1) };
		Core::ControlRMC* pControlRMC{ pControl->GetModelC<Core::ControlRMC>() };
		pControlRMC->AddObject(pFlower);
		pControlRMC->AddObject(pWizard);
		pControlRMC->AddObject(pRoyalHighness);

		pScene->Initialize();

		Core::Factory::CreatePhysicsPlane(application.GetPhysics(), pScene->GetPxScene());

		pCube->SetIsActive(true, true);
		pCamera1->SetIsActive(true, true);
		pCamera2->SetIsActive(true, true);
		pFlower->SetIsActive(true, true);
		pWizard->SetIsActive(true, true);
		pRoyalHighness->SetIsActive(true, true);
		pControl->SetIsActive(true, true);

		Core::InstancedAnimatedModelVC* pWizardModelVC{ pWizard->GetViewC<Core::InstancedAnimatedModelVC>() };
		pWizardModelVC->SetAnimation(0);
		pWizardModelVC->SetIsAnimating(true);

		Core::AnimatedModelVC* pRoyalHighnessModelVC{ pRoyalHighness->GetViewC<Core::AnimatedModelVC>() };
		pRoyalHighnessModelVC->SetAnimation(0);
		pRoyalHighnessModelVC->SetIsAnimating(true);

		pScene->SetIsActive(true);
#ifdef SOUND
		int
			startSound{ serviceLocator.GetAudio()->AddSound("bbc_world-war-_07017169.mp3", false) },
			stopSound{ serviceLocator.GetAudio()->AddSound("bbc_peugeot-20_07055235.mp3", false) };
		serviceLocator.GetAudio()->PlaySound(startSound);
#endif
#ifdef _DEBUG
		application.GetServiceLocator().GetLogger()->Message(nullptr, Core::MsgType::Info, "Application before run");
#endif
		application.Run();
#ifdef _DEBUG
		application.GetServiceLocator().GetLogger()->Message(nullptr, Core::MsgType::Info, "Application after run");
#endif
#ifdef SOUND
		serviceLocator.GetAudio()->PlaySound(stopSound);
		std::this_thread::sleep_for(std::chrono::seconds{ 4 });
#endif

		pScene->SetIsActive(false);
#ifdef _DEBUG
		application.GetServiceLocator().GetLogger()->Message(nullptr, Core::MsgType::Info, "Application shutdown");
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
	Core::MoveObjectRMC* pMoveObject{ pFlower->AddModelC<Core::MoveObjectRMC>(false) };
	Core::InputCC* pInput{ pFlower->AddControllerC<Core::InputCC>(false) };
	pScene->GetControllerST()->Register(pInput, pMoveObject->GetName(), pMoveObject->GetCommands());
	pTransformMC->SetPosition(DirectX::XMFLOAT4{ 0.f, 0.f, 0.f, 0.f });
	//DirectX::XMVECTOR axis{ 0.f, 1.f, 0.f };
	//DirectX::XMVECTOR rotVec{ DirectX::XMQuaternionRotationAxis(axis, -float(M_PI_4)) };
	//DirectX::XMFLOAT4 rot{};
	//XMStoreFloat4(&rot, rotVec);
	//pTransformMC->SetRotation(rot);
	Core::InstancedTransformMC* pFlowerTransformMC{ pFlower->AddModelC<Core::InstancedTransformMC>(false) };
	Core::InstancedModelVC* pFlowerModelVC{ pFlower->AddViewC<Core::InstancedModelVC>("Flower/Flower", "ovm", "PosNormTanTex_iA", false, Core::Winding::CCW)};
	pFlowerModelVC->AddTexture(Core::TextureType::Albedo, "Flower/Flower_Blue.png");
	pFlowerModelVC->AddCanvas(pCanvas);
	std::vector<Core::TransformMC> transforms{};
	for (size_t i{ 0 }; i < 10000; ++i)
	{
		transforms.emplace_back(true, pFlower);
		transforms.back().SetPosition(DirectX::XMFLOAT4{ 10.f * float(i / 100 + 1), 0.f, 10.f * float(i % 100 + 1), 0.f });
	}
	pFlowerTransformMC->AddInstances(transforms);
	pFlowerModelVC->SetInstancedTransform(pFlowerTransformMC);
	return pFlower;
}

Core::Object* AddWizard(Core::Scene* pScene, Core::Canvas* pCanvas1, Core::Canvas* pCanvas2)
{
	Core::Object* pWizard{ pScene->AddObject(false) };
	Core::TransformMC* pTransformMC{ pWizard->AddModelC<Core::TransformMC>(false) };
	(pTransformMC);
	//pTransformMC->SetPosition(DirectX::XMFLOAT4{ 0.25f, 0.f, 0.f, 0.f });
	//pTransformMC->SetRotation(DirectX::XMFLOAT3{ 90.f, 0.f, 0.f }, AngleUnit::Degree);
	Core::InstancedAnimatedModelVC* pWizardModelVC{ pWizard->AddViewC<Core::InstancedAnimatedModelVC>("Wizard/Wizard", "ovm", "PosNormTanTex_iaAN", false, Core::Winding::CCW, Core::CoordSystem::RightHanded) };
	Core::InstancedTransformMC* pWizardTransformMC{ pWizard->AddModelC<Core::InstancedTransformMC>(false) };
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
	pWizardTransformMC->AddInstances(transforms);
	pWizardModelVC->SetInstancedTransform(pWizardTransformMC);
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

	Core::SteeringRMC* pSteeringRMC{ pRoyalHighness->AddModelC<Core::SteeringRMC>(false) };
	Core::BehaviourCC* pBehaviour{ pRoyalHighness->AddControllerC<Core::BehaviourCC>(false) };
	pBehaviour->SetBehaviour(Core::Behaviour::Wander);
	pBehaviour->SetSteeringRMC(pSteeringRMC);
	return pRoyalHighness;
}

Core::Object* AddControl(Core::Scene* pScene, Core::Canvas* pCanvas)
{
	Core::Object* pControl{ pScene->AddObject(false) };
	Core::ControlRMC* pControlRMC{ pControl->AddModelC<Core::ControlRMC>(false) };
	Core::InputCC* pInput{ pControl->AddControllerC<Core::InputCC>(false) };
	Core::TextVC* pTextVC{ pControl->AddViewC<Core::TextVC>(false) };
	pTextVC->SetRect(D2D1_RECT_F{ 40.f, 40.f, 680.f, 400.f });
	pTextVC->SetText(L"wasd/qe/rf: Move Camera\nArrows/Del/End/PgUp/PgDn: Move Object\nTab: Cycle Object");
	pTextVC->AddCanvas(pCanvas);
	pScene->GetControllerST()->Register(pInput, pControlRMC->GetName(), pControlRMC->GetCommands());
	return pControl;
}
