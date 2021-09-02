#include "pch.h"
#include "Factory.h"
#include "Scene.h"
#include "CameraRMC.h"
#include "MeshVC.h"
#include "TriangleVC.h"
#include "CharacterRMC.h"
#include "InputCC.h"
#include "VertexPNC.h"
#include "ControllerST.h"

using namespace Ion;

Core::Object* Core::Factory::AddCamera(Core::Scene* pScene)
{
	Core::Object* pObject{ pScene->AddObject(false) };
	Core::TransformMC* pTransform{ pObject->AddModelC<Core::TransformMC>(false) };
	Core::CameraRMC* pCamera{ pObject->AddModelC<Core::CameraRMC>(false) };
	Core::InputCC* pInput{ pObject->AddControllerC<Core::InputCC>(false) };
	(pTransform);
	pScene->GetControllerST()->Register(pInput, pCamera->GetName(), pCamera->GetCommands());
	return pObject;
}

Core::Object* Core::Factory::AddCube(Core::Scene* pScene, float width, float height, float depth, DirectX::XMFLOAT4 color)
{
	float
		halfWidth{ width / 2.f },
		halfHeight{ height / 2.f },
		halfDepth{ depth / 2.f };

	Core::Object* pObject{ pScene->AddObject(false) };
	pObject->AddModelC<Core::TransformMC>(false);
	Core::MeshVC* pMeshVC{ pObject->AddViewC<Core::MeshVC>("PosNormCol", false)};
	pObject->AddViewC<Core::TriangleVC>(false);

	// Front
	pMeshVC->AddQuadrilateral(
		Core::Quadrilateral<Core::VertexPNC>{
			Core::VertexPNC{ DirectX::XMFLOAT3{ -halfWidth, halfHeight, -halfDepth }, DirectX::XMFLOAT3{ 0.f, 0.f, -1.f }, color },
			Core::VertexPNC{ DirectX::XMFLOAT3{ halfWidth, halfHeight, -halfDepth }, DirectX::XMFLOAT3{ 0.f, 0.f, -1.f }, color },
			Core::VertexPNC{ DirectX::XMFLOAT3{ halfWidth, -halfHeight, -halfDepth }, DirectX::XMFLOAT3{ 0.f, 0.f, -1.f }, color },
			Core::VertexPNC{ DirectX::XMFLOAT3{ -halfWidth, -halfHeight, -halfDepth }, DirectX::XMFLOAT3{ 0.f, 0.f, -1.f }, color } });
	// Back
	pMeshVC->AddQuadrilateral(
		Core::Quadrilateral<Core::VertexPNC>{
			Core::VertexPNC{ DirectX::XMFLOAT3{ halfWidth, halfHeight, halfDepth }, DirectX::XMFLOAT3{ 0.f, 0.f, 1.f }, color },
			Core::VertexPNC{ DirectX::XMFLOAT3{ -halfWidth, halfHeight, halfDepth }, DirectX::XMFLOAT3{ 0.f, 0.f, 1.f }, color },
			Core::VertexPNC{ DirectX::XMFLOAT3{ -halfWidth, -halfHeight, halfDepth }, DirectX::XMFLOAT3{ 0.f, 0.f, 1.f }, color },
			Core::VertexPNC{ DirectX::XMFLOAT3{ halfWidth, -halfHeight, halfDepth }, DirectX::XMFLOAT3{ 0.f, 0.f, 1.f }, color } });
	// Left
	pMeshVC->AddQuadrilateral(
		Core::Quadrilateral<Core::VertexPNC>{
			Core::VertexPNC{ DirectX::XMFLOAT3{ -halfWidth, halfHeight, halfDepth }, DirectX::XMFLOAT3{ -1.f, 0.f, 0.f }, color },
			Core::VertexPNC{ DirectX::XMFLOAT3{ -halfWidth, halfHeight, -halfDepth }, DirectX::XMFLOAT3{ -1.f, 0.f, 0.f }, color },
			Core::VertexPNC{ DirectX::XMFLOAT3{ -halfWidth, -halfHeight, -halfDepth }, DirectX::XMFLOAT3{ -1.f, 0.f, 0.f }, color },
			Core::VertexPNC{ DirectX::XMFLOAT3{ -halfWidth, -halfHeight, halfDepth }, DirectX::XMFLOAT3{ -1.f, 0.f, 0.f }, color } });
	// Right
	pMeshVC->AddQuadrilateral(
		Core::Quadrilateral<Core::VertexPNC>{
			Core::VertexPNC{ DirectX::XMFLOAT3{ halfWidth, halfHeight, -halfDepth }, DirectX::XMFLOAT3{ 1.f, 0.f, 0.f }, color },
			Core::VertexPNC{ DirectX::XMFLOAT3{ halfWidth, halfHeight, halfDepth }, DirectX::XMFLOAT3{ 1.f, 0.f, 0.f }, color },
			Core::VertexPNC{ DirectX::XMFLOAT3{ halfWidth, -halfHeight, halfDepth }, DirectX::XMFLOAT3{ 1.f, 0.f, 0.f }, color },
			Core::VertexPNC{ DirectX::XMFLOAT3{ halfWidth, -halfHeight, -halfDepth }, DirectX::XMFLOAT3{ 1.f, 0.f, 0.f }, color } });
	// Top
	pMeshVC->AddQuadrilateral(
		Core::Quadrilateral<Core::VertexPNC>{
			Core::VertexPNC{ DirectX::XMFLOAT3{ -halfWidth, halfHeight, halfDepth }, DirectX::XMFLOAT3{ 0.f, 1.f, 0.f }, color },
			Core::VertexPNC{ DirectX::XMFLOAT3{ halfWidth, halfHeight, halfDepth }, DirectX::XMFLOAT3{ 0.f, 1.f, 0.f }, color },
			Core::VertexPNC{ DirectX::XMFLOAT3{ halfWidth, halfHeight, -halfDepth }, DirectX::XMFLOAT3{ 0.f, 1.f, 0.f }, color },
			Core::VertexPNC{ DirectX::XMFLOAT3{ -halfWidth, halfHeight, -halfDepth }, DirectX::XMFLOAT3{ 0.f, 1.f, 0.f }, color } });
	// Bottom
	pMeshVC->AddQuadrilateral(
		Core::Quadrilateral<Core::VertexPNC>{
			Core::VertexPNC{ DirectX::XMFLOAT3{ -halfWidth, -halfHeight, -halfDepth }, DirectX::XMFLOAT3{ 0.f, -1.f, 0.f }, color },
			Core::VertexPNC{ DirectX::XMFLOAT3{ halfWidth, -halfHeight, -halfDepth }, DirectX::XMFLOAT3{ 0.f, -1.f, 0.f }, color },
			Core::VertexPNC{ DirectX::XMFLOAT3{ halfWidth, -halfHeight, halfDepth }, DirectX::XMFLOAT3{ 0.f, -1.f, 0.f }, color },
			Core::VertexPNC{ DirectX::XMFLOAT3{ -halfWidth, -halfHeight, halfDepth }, DirectX::XMFLOAT3{ 0.f, -1.f, 0.f }, color } });
	return pObject;
}

void Core::Factory::CreatePhysicsPlane(physx::PxPhysics* pPxPhysics, physx::PxScene* pPxScene)
{
	physx::PxRigidStatic* pPxRigidStatic{ pPxPhysics->createRigidStatic(physx::PxTransform{ physx::PxVec3{ 0.f, 0.f, 0.f } }) };
	physx::PxMaterial* pPxMaterial{ pPxPhysics->createMaterial(0.5f, 0.5f, 0.1f) };
	physx::PxShape* pShape{ pPxPhysics->createShape(physx::PxPlaneGeometry{}, *pPxMaterial) };
	physx::PxPlane pxPlane{ {0.0f, 1.0f, 0.0f}, 0.0f };
	physx::PxTransform pxTransformPlane{ PxTransformFromPlaneEquation(pxPlane) };
	pShape->setLocalPose(pxTransformPlane);
	pPxRigidStatic->attachShape(*pShape);
	pPxScene->addActor(*pPxRigidStatic);
}
