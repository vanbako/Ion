#include "pch.h"
#include "Application.h"
#include "Factory.h"
#include "MeshVC.h"
#include "ModelVC.h"
#include "InstancedTransformMC.h"
#include "TerrainVC.h"
#include "TriangleVC.h"
#include "TextVC.h"
#include "InstancedMVC.h"
#include "AnimatedMVC.h"
#include "InstancedAnimatedMVC.h"
#include "TerrainVC.h"
#include "MoveObjectRMC.h"
#include "ControlRMC.h"
#include "ObjectSteeringRMC.h"
#include "InstancedSteeringRMC.h"
#include "InputCC.h"
#include "BehaviourCC.h"
#include "ControllerST.h"
#include "Object.h"
#include "FileLogger.h"
#include "include/FmodAudio.h"
#include <iostream>

//#define TEST_SOUND
//#define SCENE1
#define SCENE2
using namespace Ion;

Core::Object* AddCamera(Core::Scene* pScene, Core::Canvas* pCanvas);
Core::Object* AddTerrain(Core::Scene* pScene, Core::Canvas* pCanvas1);
Core::Object* AddCube(Core::Scene* pScene, Core::Canvas* pCanvas1, Core::Canvas* pCanvas2 = nullptr);
Core::Object* AddFlower(Core::Scene* pScene, Core::Canvas* pCanvas);
Core::Object* AddWizard(Core::Scene* pScene, Core::Canvas* pCanvas1, Core::Canvas* pCanvas2 = nullptr);
Core::Object* AddRoyalHighness(Core::Scene* pScene, Core::Canvas* pCanvas);
Core::Object* AddControl(Core::Scene* pScene, Core::Canvas* pCanvas);
Core::Scene* AddSceneOne(Core::Application* pApplication, Core::Canvas* pCanvas1, Core::Canvas* pCanvas2);
Core::Scene* AddSceneTwo(Core::Application* pApplication, Core::Canvas* pCanvas1);

int main()
{
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

#ifdef SCENE1
		Core::Window* pWindow2{ application.AddWindow(L"Render Window 2", rectangle) };
		Core::Canvas* pCanvas2{ pWindow2->AddCanvas(rectangle) };
		pCanvas2->Initialize();
#endif
		Core::Window* pWindow1{ application.AddWindow(L"Render Window 1", rectangle) };
		Core::Canvas* pCanvas1{ pWindow1->AddCanvas(rectangle) };
		pCanvas1->Initialize();
#ifdef SCENE1
		Core::Scene* pScene1{ AddSceneOne(&application, pCanvas1, pCanvas2) };
		pScene1->Initialize();
		pScene1->SetIsActive(true);
#endif
#ifdef SCENE2
		Core::Scene* pScene2{ AddSceneTwo(&application, pCanvas1) };
		pScene2->Initialize();
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

Core::Object* AddCamera(Core::Scene* pScene, Core::Canvas* pCanvas)
{
	Core::Object* pCamera{ Core::Factory::AddCamera(pScene) };
	Core::TransformMC* pCameraTransform{ pCamera->GetModelC<Core::TransformMC>() };
	pCameraTransform->SetPosition(DirectX::XMFLOAT4{ 0.f, 20.f, -150.f, 0.f });
	pCameraTransform->SetRotation(DirectX::XMFLOAT3{ 0.f, 0.f, 0.f }, Core::AngleUnit::Degree);
	pCanvas->SetCamera(pCamera);
	return pCamera;
}

Core::Object* AddTerrain(Core::Scene* pScene, Core::Canvas* pCanvas1)
{
	Core::Object* pTerrain{ pScene->AddObject(false) };
	Core::TransformMC* pTransformMC{ pTerrain->AddModelC<Core::TransformMC>(false) };
	pTransformMC->SetPosition(DirectX::XMFLOAT4{ 0.f, 0.f, 0.f, 0.f });
	Core::TerrainVC* pTerrainVC{ pTerrain->AddViewC<Core::TerrainVC>("Hawai_HeightMap_128x128x16.raw", 2048.f, 2048.f, 128, 128, false) };
	pTerrainVC->AddTexture(Core::TextureType::Albedo, "Terrain/Hawai_TexMap.png");
	pTerrainVC->AddCanvas(pCanvas1);

	return pTerrain;
}

Core::Object* AddCube(Core::Scene* pScene, Core::Canvas* pCanvas1, Core::Canvas* pCanvas2)
{
	Core::Object* pCube{ Core::Factory::AddCube(pScene) };
	pCube->GetViewC<Core::MeshVC>()->AddCanvas(pCanvas1);
	Core::TransformMC* pTransformMC{ pCube->GetModelC<Core::TransformMC>() };
	pTransformMC->SetPosition(DirectX::XMFLOAT4{ 20.f, 160.0f, 0.f, 0.f });

	physx::PxPhysics* pPxPhysics{ &pScene->GetPxScene()->getPhysics() };
	physx::PxRigidDynamic* pPxRigidDynamic{ pPxPhysics->createRigidDynamic(physx::PxTransform{ physx::PxVec3{ 20.f, 160.f, 0.f } }) };
	physx::PxMaterial* pPxMaterial{ pPxPhysics->createMaterial(0.5f, 0.5f, 0.1f) };
	physx::PxShape* pShape{ pPxPhysics->createShape(physx::PxBoxGeometry{ 0.5f, 0.5f, 0.5f }, *pPxMaterial) };
	pPxRigidDynamic->attachShape(*pShape);
	pScene->GetPxScene()->addActor(*pPxRigidDynamic);
	pPxRigidDynamic->wakeUp();

	pTransformMC->SetPxRigidActor(pPxRigidDynamic);
	pCube->GetViewC<Core::TriangleVC>()->AddCanvas(pCanvas1);
	if (pCanvas2 != nullptr)
		pCube->GetViewC<Core::TriangleVC>()->AddCanvas(pCanvas2);
	return pCube;
}

Core::Object* AddFlower(Core::Scene* pScene, Core::Canvas* pCanvas)
{
	Core::Object* pFlower{ pScene->AddObject(false) };
	Core::TransformMC* pTransformMC{ pFlower->AddModelC<Core::TransformMC>(false) };
	//Core::MoveObjectRMC* pMoveObject{ pFlower->AddModelC<Core::MoveObjectRMC>(false) };
	//Core::InputCC* pInput{ pFlower->AddControllerC<Core::InputCC>(false) };
	//pScene->GetControllerST()->Register(pInput, pMoveObject->GetName(), pMoveObject->GetCommands());
	pTransformMC->SetPosition(DirectX::XMFLOAT4{ 0.f, 0.f, 0.f, 0.f });
	//DirectX::XMVECTOR axis{ 0.f, 1.f, 0.f };
	//DirectX::XMVECTOR rotVec{ DirectX::XMQuaternionRotationAxis(axis, -float(M_PI_4)) };
	//DirectX::XMFLOAT4 rot{};
	//XMStoreFloat4(&rot, rotVec);
	//pTransformMC->SetRotation(rot);
	Core::InstancedTransformMC* pFlowerTransformMC{ pFlower->AddModelC<Core::InstancedTransformMC>(false) };
	Core::InstancedMVC* pFlowerModelVC{ pFlower->AddViewC<Core::InstancedMVC>("Flower/Flower", "ovm", "PosNormTanTex_iA", false, Core::Winding::CCW)};
	pFlowerModelVC->AddTexture(Core::TextureType::Albedo, "Flower/Flower_Blue.png");
	pFlowerModelVC->AddCanvas(pCanvas);
	std::vector<Core::TransformMC> transforms{};
	std::vector<Core::Behaviour> behaviours{};
	DirectX::XMFLOAT4 pos{};
	float scale{ 1.f };
	for (size_t i{ 0 }; i < 16384; ++i)
	{
		Core::TransformMC& transformMC{ transforms.emplace_back(true, pFlower) };
		pos.x = float((long long(i) / 128 - 64) * 16) + float(std::rand() % 11) - 5.f;
		pos.z = float((long long(i) % 128 - 64) * 16) + float(std::rand() % 11) - 5.f;
		transformMC.SetPosition(pos);
		scale = 0.5f + float(std::rand() % 1000) / 1000.f;
		transformMC.SetScale(DirectX::XMFLOAT4{ scale, scale, scale, 0.f });
		transformMC.SetRotation(DirectX::XMFLOAT3{ 0.f, float(std::rand() % 360), 0.f });
	}
	pFlowerTransformMC->AddInstances(transforms, behaviours);
	pFlowerModelVC->SetInstancedTransform(pFlowerTransformMC);
	return pFlower;
}

Core::Object* AddWizard(Core::Scene* pScene, Core::Canvas* pCanvas1, Core::Canvas* pCanvas2)
{
	physx::PxPhysics* pPxPhysics{ pScene->GetApplication()->GetPxPhysics() };
	physx::PxControllerManager* pPxControllerManager{ pScene->GetPxControllerManager() };
	physx::PxCapsuleControllerDesc ccDesc{};
	ccDesc.radius = 10.f;
	ccDesc.height = 20.f;
	ccDesc.upDirection = physx::PxVec3{ 0.f, 1.f, 0.f };
	Core::Object* pWizard{ pScene->AddObject(false) };
	Core::TransformMC* pTransformMC{ pWizard->AddModelC<Core::TransformMC>(false) };
	(pTransformMC);
	//pTransformMC->SetPosition(DirectX::XMFLOAT4{ 0.25f, 0.f, 0.f, 0.f });
	//pTransformMC->SetRotation(DirectX::XMFLOAT3{ 90.f, 0.f, 0.f }, AngleUnit::Degree);
	Core::InstancedAnimatedMVC* pWizardModelVC{ pWizard->AddViewC<Core::InstancedAnimatedMVC>("Wizard/Wizard", "ovm", "PosNormTanTex_iaAN", false, Core::Winding::CCW, Core::CoordSystem::RightHanded) };
	Core::InstancedTransformMC* pWizardTransformMC{ pWizard->AddModelC<Core::InstancedTransformMC>(false) };
	pWizardModelVC->AddTexture(Core::TextureType::Albedo, "Wizard/Wizard_Blue_A.png");
	pWizardModelVC->AddTexture(Core::TextureType::Normal, "Wizard/Wizard_Blue_N.png");
	pWizardModelVC->AddCanvas(pCanvas1);
	if (pCanvas2 != nullptr)
		pWizardModelVC->AddCanvas(pCanvas2);
	//TransformMC transformMC{ true, pWizard };
	//transformMC.SetPosition(DirectX::XMFLOAT4{ 10.f, 0.f, 0.f, 0.f });
	//transformMC.SetRotation(DirectX::XMFLOAT3{ 90.f, 0.f, 0.f }, AngleUnit::Degree);
	//pWizardModelVC->AddInstance(transformMC);
	//transformMC.SetPosition(DirectX::XMFLOAT4{ -10.f, 0.f, 0.f, 0.f });
	//transformMC.SetRotation(DirectX::XMFLOAT3{ 90.f, 0.f, 0.f }, AngleUnit::Degree);
	//pWizardModelVC->AddInstance(transformMC);
	std::vector<Core::TransformMC> transforms{};
	std::vector<Core::Behaviour> behaviours{};
	physx::PxController* pPxController{ nullptr };
	DirectX::XMFLOAT4 pos{};
	physx::PxMaterial* pPxMaterial{ pPxPhysics->createMaterial(0.5f, 0.5f, 0.1f) };
	std::size_t maxWizard{ 20 };
	for (std::size_t i{ 0 }; i < maxWizard; ++i)
	{
		pos = DirectX::XMFLOAT4{ 40.f * float(int(i) - int(maxWizard / 2)), 150.f, 0.f, 0.f };
		ccDesc.position = physx::PxExtendedVec3{ physx::PxExtended(pos.x), physx::PxExtended(pos.y), physx::PxExtended(pos.z) };
		ccDesc.material = pPxMaterial;
		pPxController = pPxControllerManager->createController(ccDesc);
		Core::TransformMC& transformMC{ transforms.emplace_back(true, pWizard) };
		transformMC.SetPosition(pos);
		transformMC.SetPxController(pPxController);
		behaviours.emplace_back(Core::Behaviour::Wander);
	}
	behaviours[0] = Core::Behaviour::Seek;
	pWizardTransformMC->SetHasBehaviour(true);
	pWizardTransformMC->AddInstances(transforms, behaviours);
	pWizardModelVC->SetInstancedTransform(pWizardTransformMC);

	pWizardTransformMC->SetIsStatic(false);
	Core::InstancedSteeringRMC* pInstancedSteeringRMC{ pWizard->AddModelC<Core::InstancedSteeringRMC>(false) };
	Core::BehaviourCC* pWanderBehaviour{ pWizard->AddControllerC<Core::BehaviourCC>(false) };
	pWanderBehaviour->SetBehaviour(Core::Behaviour::Wander);
	pWanderBehaviour->SetSteeringRMC(pInstancedSteeringRMC);
	Core::BehaviourCC* pSeekBehaviour{ pWizard->AddControllerC<Core::BehaviourCC>(false) };
	pSeekBehaviour->SetBehaviour(Core::Behaviour::Seek);
	pSeekBehaviour->SetSteeringRMC(pInstancedSteeringRMC);
	pInstancedSteeringRMC->SetInstancedTransformMC(pWizardTransformMC);
	pInstancedSteeringRMC->SetTarget(0, &pWizardTransformMC->GetInstances()[maxWizard - 1]);

	return pWizard;
}

Core::Object* AddRoyalHighness(Core::Scene* pScene, Core::Canvas* pCanvas)
{
	Core::Object* pRoyalHighness{ pScene->AddObject(false) };
	Core::TransformMC* pTransformMC{ pRoyalHighness->AddModelC<Core::TransformMC>(false) };
	pTransformMC->SetPosition(DirectX::XMFLOAT4{ 0.f, 0.f, -25.f, 0.f });
	//pTransformMC->SetScale(DirectX::XMFLOAT4{ 0.5f, 0.5f, 0.5f, 0.f });
	//pTransformMC->SetRotation(DirectX::XMFLOAT3{ 90.f, 0.f, 0.f }, Core::AngleUnit::Degree);
	Core::AnimatedMVC* pRoyalHighnessModelVC{ pRoyalHighness->AddViewC<Core::AnimatedMVC>("RoyalHighness/RoyalHighness1", "ovm", "PosNormTanTex_aAN", false, Core::Winding::CCW, Core::CoordSystem::RightHanded) };
	//ModelVC* pRoyalHighnessModelVC{ pRoyalHighness->AddViewC<ModelVC>("RoyalHighness/RoyalHighness1", "PosNormTanTex_AN", false, Winding::CCW, CoordSystem::RightHanded) };
	pRoyalHighnessModelVC->AddTexture(Core::TextureType::Albedo, "RoyalHighness/RoyalHighness_Red_A.png");
	pRoyalHighnessModelVC->AddTexture(Core::TextureType::Normal, "RoyalHighness/RoyalHighness_Red_N.png");
	pRoyalHighnessModelVC->AddCanvas(pCanvas);

	Core::ObjectSteeringRMC* pObjectSteeringRMC{ pRoyalHighness->AddModelC<Core::ObjectSteeringRMC>(false) };
	Core::BehaviourCC* pBehaviour{ pRoyalHighness->AddControllerC<Core::BehaviourCC>(false) };
	pBehaviour->SetBehaviour(Core::Behaviour::Wander);
	pBehaviour->SetSteeringRMC(pObjectSteeringRMC);
	return pRoyalHighness;
}

Core::Object* AddControl(Core::Scene* pScene, Core::Canvas* pCanvas)
{
	Core::Object* pControl{ pScene->AddObject(false) };
	Core::ControlRMC* pControlRMC{ pControl->AddModelC<Core::ControlRMC>(false) };
	Core::InputCC* pInput{ pControl->AddControllerC<Core::InputCC>(false) };
	Core::TextVC* pTextVC{ pControl->AddViewC<Core::TextVC>(false) };
	pTextVC->SetRect(D2D1_RECT_F{ 40.f, 40.f, 680.f, 400.f });
	//pTextVC->SetText(L"wasd/qe/rf: Move Camera\nArrows/Del/End/PgUp/PgDn: Move Object\nTab: Cycle Object");
	pTextVC->SetText(L"wasd/qe/rf: Move Camera");
	pTextVC->AddCanvas(pCanvas);
	pScene->GetControllerST()->Register(pInput, pControlRMC->GetName(), pControlRMC->GetCommands());
	return pControl;
}

Core::Scene* AddSceneOne(Core::Application* pApplication, Core::Canvas* pCanvas1, Core::Canvas* pCanvas2)
{
	Core::Scene* pScene{ pApplication->AddScene() };

	pScene->AddCanvas(pCanvas2);
	pScene->AddCanvas(pCanvas1);

	Core::Object* pCube{ AddCube(pScene, pCanvas1, pCanvas2) };
	Core::Object* pFlower{ AddFlower(pScene, pCanvas1) };
	Core::Object* pWizard{ AddWizard(pScene, pCanvas1, pCanvas2) };
	Core::Object* pRoyalHighness{ AddRoyalHighness(pScene, pCanvas1) };
	Core::Object* pCamera1{ AddCamera(pScene, pCanvas1) };
	Core::Object* pCamera2{ AddCamera(pScene, pCanvas2) };
	Core::Object* pControl{ AddControl(pScene, pCanvas1) };
	Core::ControlRMC* pControlRMC{ pControl->GetModelC<Core::ControlRMC>() };
	pControlRMC->AddObject(pFlower);
	Core::MoveObjectRMC* pMoveObject{ pFlower->AddModelC<Core::MoveObjectRMC>(false) };
	Core::InputCC* pInput{ pFlower->AddControllerC<Core::InputCC>(false) };
	pScene->GetControllerST()->Register(pInput, pMoveObject->GetName(), pMoveObject->GetCommands());
	pControlRMC->AddObject(pWizard);
	pControlRMC->AddObject(pRoyalHighness);

	pScene->Initialize();

	Core::Factory::CreatePhysicsPlane(pApplication->GetPxPhysics(), pScene->GetPxScene());

	pCube->SetIsActive(true, true);
	pCamera1->SetIsActive(true, true);
	pCamera2->SetIsActive(true, true);
	pFlower->SetIsActive(true, true);
	pWizard->SetIsActive(true, true);
	pRoyalHighness->SetIsActive(true, true);
	pControl->SetIsActive(true, true);

	Core::InstancedAnimatedMVC* pWizardModelVC{ pWizard->GetViewC<Core::InstancedAnimatedMVC>() };
	pWizardModelVC->SetAnimation(0);
	pWizardModelVC->SetIsAnimating(true);

	Core::AnimatedMVC* pRoyalHighnessModelVC{ pRoyalHighness->GetViewC<Core::AnimatedMVC>() };
	pRoyalHighnessModelVC->SetAnimation(0);
	pRoyalHighnessModelVC->SetIsAnimating(true);
	return pScene;
}

Core::Scene* AddSceneTwo(Core::Application* pApplication, Core::Canvas* pCanvas1)
{
	Core::Scene* pScene{ pApplication->AddScene() };

	pScene->AddCanvas(pCanvas1);

	Core::Object* pTerrain{ AddTerrain(pScene, pCanvas1) };
	Core::Object* pCube{ AddCube(pScene, pCanvas1) };
	Core::Object* pFlower{ AddFlower(pScene, pCanvas1) };
	Core::Object* pWizard{ AddWizard(pScene, pCanvas1) };
	Core::Object* pCamera1{ AddCamera(pScene, pCanvas1) };
	pCamera1->GetModelC<Core::TransformMC>()->SetPosition(DirectX::XMFLOAT4{ 0.f, 120.f, -400.f, 0.f });
	Core::Object* pControl{ AddControl(pScene, pCanvas1) };
	Core::ControlRMC* pControlRMC{ pControl->GetModelC<Core::ControlRMC>() };
	pControlRMC->AddObject(pWizard);
	Core::MoveObjectRMC* pMoveObject{ pWizard->AddModelC<Core::MoveObjectRMC>(false) };
	Core::InputCC* pInput{ pWizard->AddControllerC<Core::InputCC>(false) };
	pScene->GetControllerST()->Register(pInput, pMoveObject->GetName(), pMoveObject->GetCommands());

	pScene->Initialize();

	Core::InstancedTransformMC* pInstancedTransformMC{ pFlower->GetModelC<Core::InstancedTransformMC>() };
	pInstancedTransformMC->ApplyTerrain(pTerrain->GetViewC<Core::TerrainVC>());
	pFlower->GetViewC<Core::InstancedMVC>()->SetInstancedTransform(pInstancedTransformMC);

	Core::Factory::CreatePhysicsPlane(pApplication->GetPxPhysics(), pScene->GetPxScene());

	pCube->SetIsActive(true, true);
	pCamera1->SetIsActive(true, true);
	pTerrain->SetIsActive(true, true);
	pWizard->SetIsActive(true, true);
	pFlower->SetIsActive(true, true);
	pControl->SetIsActive(true, true);

	Core::InstancedAnimatedMVC* pWizardModelVC{ pWizard->GetViewC<Core::InstancedAnimatedMVC>() };
	pWizardModelVC->SetAnimation(0);
	pWizardModelVC->SetIsAnimating(true);

	return pScene;
}
