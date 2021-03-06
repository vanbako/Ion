#include "pch.h"
#include "../Core/include/Application.h"
#include "../Core/include/Factory.h"
#include "../Test/include/Factory.h"
#include "../Core/include/Scene.h"
#include "../Core/include/InstancedTransformMC.h"
#include "../Core/include/InstancedAnimatedMVC.h"
#include "../Core/include/CameraRMC.h"
#include "../Core/include/InstancedSteeringRMC.h"
#include "../Core/include/MeshVC.h"
#include "../Core/include/AnimatedMVC.h"
#include "../Core/include/TriangleVC.h"
#include "../Core/include/TerrainVC.h"
#include "../Core/include/CharacterRMC.h"
#include "../Core/include/ObjectSteeringRMC.h"
#include "../Test/include/ControlZeroRMC.h"
#include "../Test/include/ControlOneRMC.h"
#include "../Core/include/MoveObjectRMC.h"
#include "../Core/include/InputCC.h"
#include "../Core/include/VertexPNC.h"
#include "../Core/include/ControllerST.h"
#include "../Core/include/BehaviourCC.h"

using namespace Ion;

Core::Object* Test::Factory::AddCamera(Core::Scene* pScene, Core::Canvas* pCanvas)
{
	Core::Object* pCamera{ Core::Factory::AddCamera(pScene) };
	Core::TransformMC* pCameraTransform{ pCamera->GetModelC<Core::TransformMC>() };
	pCameraTransform->SetPosition(DirectX::XMFLOAT4{ 0.f, 20.f, -150.f, 0.f });
	pCameraTransform->SetRotation(DirectX::XMFLOAT3{ 0.f, 0.f, 0.f }, Core::AngleUnit::Degree);
	pCanvas->SetCamera(pCamera);
	return pCamera;
}

Core::Object* Test::Factory::AddTerrain(Core::Scene* pScene, Core::Canvas* pCanvas)
{
	Core::Object* pTerrain{ pScene->AddObject(false) };
	Core::TransformMC* pTransformMC{ pTerrain->AddModelC<Core::TransformMC>(false) };
	pTransformMC->SetPosition(DirectX::XMFLOAT4{ 0.f, 0.f, 0.f, 0.f });
	Core::TerrainVC* pTerrainVC{ pTerrain->AddViewC<Core::TerrainVC>("Hawai_HeightMap_128x128x16.raw", 2048.f, 2048.f, 128, 128, false) };
	pTerrainVC->AddTexture(Core::TextureType::Albedo, "Terrain/Hawai_TexMap.png");
	pTerrainVC->AddCanvas(pCanvas);

	return pTerrain;
}

Core::Object* Test::Factory::AddCube(Core::Scene* pScene, Core::Canvas* pCanvas)
{
	Core::Object* pCube{ Core::Factory::AddCube(pScene) };
	pCube->GetViewC<Core::MeshVC>()->AddCanvas(pCanvas);
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
	pCube->GetViewC<Core::TriangleVC>()->AddCanvas(pCanvas);
	return pCube;
}

Core::Object* Test::Factory::AddFlowerOne(Core::Scene* pScene, Core::Canvas* pCanvas)
{
	Core::Object* pFlower{ pScene->AddObject(false) };
	Core::TransformMC* pTransformMC{ pFlower->AddModelC<Core::TransformMC>(false) };
	pTransformMC->SetPosition(DirectX::XMFLOAT4{ 0.f, 0.f, 0.f, 0.f });
	Core::InstancedTransformMC* pFlowerTransformMC{ pFlower->AddModelC<Core::InstancedTransformMC>(false) };
	Core::InstancedMVC* pFlowerModelVC{ pFlower->AddViewC<Core::InstancedMVC>("Flower/Flower", "ovm", "PosNormTanTex_iA", false, Core::Winding::CCW) };
	pFlowerModelVC->AddTexture(Core::TextureType::Albedo, "Flower/Flower_Blue.png");
	pFlowerModelVC->AddCanvas(pCanvas);
	pFlowerModelVC->SetShininess(0.f);
	std::vector<Core::TransformMC> transforms{};
	std::vector<Core::Behaviour> behaviours{};
	DirectX::XMFLOAT4 pos{};
	float scale{ 1.f };
	for (std::size_t i{ 0 }; i < 256; ++i)
	{
		Core::TransformMC& transformMC{ transforms.emplace_back(true, pFlower) };
		pos.x = float((long long(i) / 16 - 8) * 16) + float(std::rand() % 11) - 5.f;
		pos.z = float((long long(i) % 16 - 8) * 16) + float(std::rand() % 11) - 5.f;
		transformMC.SetPosition(pos);
		scale = 0.5f + float(std::rand() % 1000) / 1000.f;
		transformMC.SetScale(DirectX::XMFLOAT4{ scale, scale, scale, 0.f });
		transformMC.SetRotation(DirectX::XMFLOAT3{ 0.f, float(std::rand() % 360), 0.f });
	}
	pFlowerTransformMC->AddInstances(transforms, behaviours);
	pFlowerModelVC->SetInstancedTransform(pFlowerTransformMC);
	return pFlower;
}

Core::Object* Test::Factory::AddFlowerTwo(Core::Scene* pScene, Core::Canvas* pCanvas)
{
	Core::Object* pFlower{ pScene->AddObject(false) };
	Core::TransformMC* pTransformMC{ pFlower->AddModelC<Core::TransformMC>(false) };
	pTransformMC->SetPosition(DirectX::XMFLOAT4{ 0.f, 0.f, 0.f, 0.f });
	Core::InstancedTransformMC* pFlowerTransformMC{ pFlower->AddModelC<Core::InstancedTransformMC>(false) };
	Core::InstancedMVC* pFlowerModelVC{ pFlower->AddViewC<Core::InstancedMVC>("Flower/Flower", "ovm", "PosNormTanTex_iA", false, Core::Winding::CCW) };
	pFlowerModelVC->AddTexture(Core::TextureType::Albedo, "Flower/Flower_Blue.png");
	pFlowerModelVC->AddCanvas(pCanvas);
	pFlowerModelVC->SetShininess(0.f);
	std::vector<Core::TransformMC> transforms{};
	std::vector<Core::Behaviour> behaviours{};
	DirectX::XMFLOAT4 pos{};
	float scale{ 1.f };
	for (std::size_t i{ 0 }; i < 16384; ++i)
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

Core::Object* Test::Factory::AddWizard(Core::Scene* pScene, Core::Canvas* pCanvas)
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
	Core::InstancedAnimatedMVC* pWizardModelVC{ pWizard->AddViewC<Core::InstancedAnimatedMVC>("Wizard/Wizard", "ovm", "PosNormTanTex_iaAN", false, Core::Winding::CCW, Core::CoordSystem::RightHanded) };
	Core::InstancedTransformMC* pWizardTransformMC{ pWizard->AddModelC<Core::InstancedTransformMC>(false) };
	pWizardModelVC->AddTexture(Core::TextureType::Albedo, "Wizard/Wizard_Blue_A.png");
	pWizardModelVC->AddTexture(Core::TextureType::Normal, "Wizard/Wizard_Blue_N.png");
	pWizardModelVC->AddCanvas(pCanvas);
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

Core::Object* Test::Factory::AddRoyalHighness(Core::Scene* pScene, Core::Canvas* pCanvas)
{
	Core::Object* pRoyalHighness{ pScene->AddObject(false) };
	Core::TransformMC* pTransformMC{ pRoyalHighness->AddModelC<Core::TransformMC>(false) };
	pTransformMC->SetPosition(DirectX::XMFLOAT4{ 0.f, 0.f, -25.f, 0.f });
	Core::AnimatedMVC* pRoyalHighnessModelVC{ pRoyalHighness->AddViewC<Core::AnimatedMVC>("RoyalHighness/RoyalHighness1", "ovm", "PosNormTanTex_aAN", false, Core::Winding::CCW, Core::CoordSystem::RightHanded) };
	pRoyalHighnessModelVC->AddTexture(Core::TextureType::Albedo, "RoyalHighness/RoyalHighness_Red_A.png");
	pRoyalHighnessModelVC->AddTexture(Core::TextureType::Normal, "RoyalHighness/RoyalHighness_Red_N.png");
	pRoyalHighnessModelVC->AddCanvas(pCanvas);

	Core::ObjectSteeringRMC* pObjectSteeringRMC{ pRoyalHighness->AddModelC<Core::ObjectSteeringRMC>(false) };
	Core::BehaviourCC* pBehaviour{ pRoyalHighness->AddControllerC<Core::BehaviourCC>(false) };
	pBehaviour->SetBehaviour(Core::Behaviour::Wander);
	pBehaviour->SetSteeringRMC(pObjectSteeringRMC);
	return pRoyalHighness;
}

Core::Scene* Test::Factory::AddSceneOne(Core::Application* pApplication, Core::Canvas* pCanvas)
{
	Core::Scene* pScene{ pApplication->AddScene("SceneOne") };

	pScene->AddCanvas(pCanvas);

	Core::Object* pCube{ AddCube(pScene, pCanvas) };
	Core::Object* pFlower{ AddFlowerOne(pScene, pCanvas) };
	Core::Object* pRoyalHighness{ AddRoyalHighness(pScene, pCanvas) };
	Core::Object* pCamera{ AddCamera(pScene, pCanvas) };

	Core::Object* pControlOne{ pScene->AddObject(false) };
	Core::ControlOneRMC* pControlOneRMC{ pControlOne->AddModelC<Core::ControlOneRMC>(false) };
	Core::InputCC* pInputControl{ pControlOne->AddControllerC<Core::InputCC>(false) };
	//Core::TextVC* pTextVC{ pControlOne->AddViewC<Core::TextVC>(false) };
	//pTextVC->SetRect(D2D1_RECT_F{ 40.f, 40.f, 680.f, 400.f });
	//pTextVC->SetText(L"wasd/qe/rf: Move Camera\nArrows/Del/End/PgUp/PgDn: Move Object\nTab: Cycle Object");
	//pTextVC->AddCanvas(pCanvas);
	pControlOneRMC->AddObject(pFlower);
	pControlOneRMC->AddObject(pRoyalHighness);
	pScene->GetControllerST()->Register(pInputControl, pControlOneRMC->GetName(), pControlOneRMC->GetCommands());

	Core::MoveObjectRMC* pMoveObject{ pFlower->AddModelC<Core::MoveObjectRMC>(false) };
	Core::InputCC* pInputMove{ pFlower->AddControllerC<Core::InputCC>(false) };
	pScene->GetControllerST()->Register(pInputMove, pMoveObject->GetName(), pMoveObject->GetCommands());

	pScene->Initialize();

	Core::Factory::CreatePhysicsPlane(pApplication->GetPxPhysics(), pScene->GetPxScene());

	pCube->SetIsActive(true, true);
	pCamera->SetIsActive(true, true);
	pFlower->SetIsActive(true, true);
	pRoyalHighness->SetIsActive(true, true);
	pControlOne->SetIsActive(true, true);

	Core::AnimatedMVC* pRoyalHighnessModelVC{ pRoyalHighness->GetViewC<Core::AnimatedMVC>() };
	pRoyalHighnessModelVC->SetAnimation(0);
	pRoyalHighnessModelVC->SetIsAnimating(true);
	return pScene;
}

Core::Scene* Test::Factory::AddSceneTwo(Core::Application* pApplication, Core::Canvas* pCanvas)
{
	Core::Scene* pScene{ pApplication->AddScene("SceneTwo") };

	pScene->AddCanvas(pCanvas);

	Core::Object* pTerrain{ AddTerrain(pScene, pCanvas) };
	Core::Object* pCube{ AddCube(pScene, pCanvas) };
	Core::Object* pFlower{ AddFlowerTwo(pScene, pCanvas) };
	Core::Object* pWizard{ AddWizard(pScene, pCanvas) };
	Core::Object* pCamera{ AddCamera(pScene, pCanvas) };
	pCamera->GetModelC<Core::TransformMC>()->SetPosition(DirectX::XMFLOAT4{ 0.f, 120.f, -400.f, 0.f });

	pScene->Initialize();

	Core::InstancedTransformMC* pInstancedTransformMC{ pFlower->GetModelC<Core::InstancedTransformMC>() };
	pInstancedTransformMC->ApplyTerrain(pTerrain->GetViewC<Core::TerrainVC>());
	pFlower->GetViewC<Core::InstancedMVC>()->SetInstancedTransform(pInstancedTransformMC);

	Core::Factory::CreatePhysicsPlane(pApplication->GetPxPhysics(), pScene->GetPxScene());

	pCube->SetIsActive(true, true);
	pCamera->SetIsActive(true, true);
	pTerrain->SetIsActive(true, true);
	pWizard->SetIsActive(true, true);
	pFlower->SetIsActive(true, true);

	Core::InstancedAnimatedMVC* pWizardModelVC{ pWizard->GetViewC<Core::InstancedAnimatedMVC>() };
	pWizardModelVC->SetAnimation(0);
	pWizardModelVC->SetIsAnimating(true);

	return pScene;
}

Core::Scene* Test::Factory::AddSceneZero(Core::Application* pApplication)
{
	Core::Scene* pScene{ pApplication->AddScene("SceneZero") };

	Core::Object* pControlZero{ pScene->AddObject(false) };
	Core::ControlZeroRMC* pControlZeroRMC{ pControlZero->AddModelC<Core::ControlZeroRMC>(false) };
	Core::InputCC* pInput{ pControlZero->AddControllerC<Core::InputCC>(false) };
	pScene->GetControllerST()->Register(pInput, pControlZeroRMC->GetName(), pControlZeroRMC->GetCommands());

	pScene->Initialize();

	pControlZero->SetIsActive(true, true);

	return pScene;
}
