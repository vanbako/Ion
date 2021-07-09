#include "../Core/pch.h"
#include "../Core/Factory.h"
#include "../Core/Scene.h"
#include "../Core/CameraRMC.h"
#include "../Core/MeshVC.h"
#include "../Core/TriangleVC.h"
#include "../Core/CharacterRMC.h"
#include "../Core/InputCC.h"
#include "../Core/VertexPNC.h"
#include "../Core/ControllerST.h"

using namespace Ion::Core;

Object* Factory::AddCamera(Scene* pScene)
{
	Object* pObject{ pScene->AddObject(false) };
	TransformMC* pTransform{ pObject->AddModelC<TransformMC>(false) };
	CameraRMC* pCamera{ pObject->AddModelC<CameraRMC>(false) };
	InputCC* pInput{ pObject->AddControllerC<InputCC>(false) };
	(pTransform);
	(pCamera);
	pScene->GetControllerST()->Register(pInput, pCamera->GetName(), pCamera->GetCommands());
	return pObject;
}

Object* Factory::AddCube(Scene* pScene, float width, float height, float depth, DirectX::XMFLOAT4 color)
{
	float halfWidth{ width / 2.f };
	float halfHeight{ height / 2.f };
	float halfDepth{ depth / 2.f };

	Object* pObject{ pScene->AddObject(false) };
	pObject->AddModelC<TransformMC>(false);
	MeshVC* pMeshVC{ pObject->AddViewC<MeshVC>(false) };
	pObject->AddViewC<TriangleVC>(false);

	// Front
	pMeshVC->AddQuadrilateral(
		Quadrilateral<VertexPNC>{
		VertexPNC{ DirectX::XMFLOAT3{ -halfWidth, halfHeight, -halfDepth }, DirectX::XMFLOAT3{ 0.f, 0.f, -1.f }, color },
			VertexPNC{ DirectX::XMFLOAT3{ halfWidth, halfHeight, -halfDepth }, DirectX::XMFLOAT3{ 0.f, 0.f, -1.f }, color },
			VertexPNC{ DirectX::XMFLOAT3{ halfWidth, -halfHeight, -halfDepth }, DirectX::XMFLOAT3{ 0.f, 0.f, -1.f }, color },
			VertexPNC{ DirectX::XMFLOAT3{ -halfWidth, -halfHeight, -halfDepth }, DirectX::XMFLOAT3{ 0.f, 0.f, -1.f }, color } });

	// Back
	pMeshVC->AddQuadrilateral(
		Quadrilateral<VertexPNC>{
		VertexPNC{ DirectX::XMFLOAT3{ halfWidth, halfHeight, halfDepth }, DirectX::XMFLOAT3{ 0.f, 0.f, 1.f }, color },
			VertexPNC{ DirectX::XMFLOAT3{ -halfWidth, halfHeight, halfDepth }, DirectX::XMFLOAT3{ 0.f, 0.f, 1.f }, color },
			VertexPNC{ DirectX::XMFLOAT3{ -halfWidth, -halfHeight, halfDepth }, DirectX::XMFLOAT3{ 0.f, 0.f, 1.f }, color },
			VertexPNC{ DirectX::XMFLOAT3{ halfWidth, -halfHeight, halfDepth }, DirectX::XMFLOAT3{ 0.f, 0.f, 1.f }, color } });

	// Left
	pMeshVC->AddQuadrilateral(
		Quadrilateral<VertexPNC>{
		VertexPNC{ DirectX::XMFLOAT3{ -halfWidth, halfHeight, halfDepth }, DirectX::XMFLOAT3{ -1.f, 0.f, 0.f }, color },
			VertexPNC{ DirectX::XMFLOAT3{ -halfWidth, halfHeight, -halfDepth }, DirectX::XMFLOAT3{ -1.f, 0.f, 0.f }, color },
			VertexPNC{ DirectX::XMFLOAT3{ -halfWidth, -halfHeight, -halfDepth }, DirectX::XMFLOAT3{ -1.f, 0.f, 0.f }, color },
			VertexPNC{ DirectX::XMFLOAT3{ -halfWidth, -halfHeight, halfDepth }, DirectX::XMFLOAT3{ -1.f, 0.f, 0.f }, color } });

	// Right
	pMeshVC->AddQuadrilateral(
		Quadrilateral<VertexPNC>{
		VertexPNC{ DirectX::XMFLOAT3{ halfWidth, halfHeight, -halfDepth }, DirectX::XMFLOAT3{ 1.f, 0.f, 0.f }, color },
			VertexPNC{ DirectX::XMFLOAT3{ halfWidth, halfHeight, halfDepth }, DirectX::XMFLOAT3{ 1.f, 0.f, 0.f }, color },
			VertexPNC{ DirectX::XMFLOAT3{ halfWidth, -halfHeight, halfDepth }, DirectX::XMFLOAT3{ 1.f, 0.f, 0.f }, color },
			VertexPNC{ DirectX::XMFLOAT3{ halfWidth, -halfHeight, -halfDepth }, DirectX::XMFLOAT3{ 1.f, 0.f, 0.f }, color } });

	// Top
	pMeshVC->AddQuadrilateral(
		Quadrilateral<VertexPNC>{
		VertexPNC{ DirectX::XMFLOAT3{ -halfWidth, halfHeight, halfDepth }, DirectX::XMFLOAT3{ 0.f, 1.f, 0.f }, color },
			VertexPNC{ DirectX::XMFLOAT3{ halfWidth, halfHeight, halfDepth }, DirectX::XMFLOAT3{ 0.f, 1.f, 0.f }, color },
			VertexPNC{ DirectX::XMFLOAT3{ halfWidth, halfHeight, -halfDepth }, DirectX::XMFLOAT3{ 0.f, 1.f, 0.f }, color },
			VertexPNC{ DirectX::XMFLOAT3{ -halfWidth, halfHeight, -halfDepth }, DirectX::XMFLOAT3{ 0.f, 1.f, 0.f }, color } });

	// Bottom
	pMeshVC->AddQuadrilateral(
		Quadrilateral<VertexPNC>{
		VertexPNC{ DirectX::XMFLOAT3{ -halfWidth, -halfHeight, -halfDepth }, DirectX::XMFLOAT3{ 0.f, -1.f, 0.f }, color },
			VertexPNC{ DirectX::XMFLOAT3{ halfWidth, -halfHeight, -halfDepth }, DirectX::XMFLOAT3{ 0.f, -1.f, 0.f }, color },
			VertexPNC{ DirectX::XMFLOAT3{ halfWidth, -halfHeight, halfDepth }, DirectX::XMFLOAT3{ 0.f, -1.f, 0.f }, color },
			VertexPNC{ DirectX::XMFLOAT3{ -halfWidth, -halfHeight, halfDepth }, DirectX::XMFLOAT3{ 0.f, -1.f, 0.f }, color } });

	return pObject;
}

void Factory::CreatePhysicsPlane(physx::PxPhysics* pPxPhysics, physx::PxScene* pPxScene)
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
