#include "../Core/pch.h"
#include "../Core/Scene.h"
#include "../Core/CameraMC.h"
#include "../Core/Factory.h"
#include "../Core/MeshVC.h"
#include "../Core/VertexPNC.h"

using namespace Ion::Core;

Object* Factory::AddCamera(Scene* pScene)
{
	Object* pObject{ pScene->AddObject(false) };
	pObject->AddModelC<TransformMC>(false);
	pObject->AddModelC<CameraMC>(false);
	return pObject;
}

void Factory::SetCameraActive(Object* pCamera)
{
	pCamera->GetModelC<TransformMC>()->SetIsActive(true);
	pCamera->GetModelC<CameraMC>()->SetIsActive(true);
	pCamera->SetIsActive(true);
}

Object* Factory::AddCube(Scene* pScene, float width, float height, float depth, DirectX::XMFLOAT4 color)
{
	float halfWidth{ width / 2.f };
	float halfHeight{ height / 2.f };
	float halfDepth{ depth / 2.f };

	Object* pObject{ pScene->AddObject(false) };
	pObject->AddModelC<TransformMC>(false);
	MeshVC* pMeshVC{ pObject->AddViewC<MeshVC>(false) };

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

void Ion::Core::Factory::SetCubeActive(Object* pCube)
{
	pCube->GetModelC<TransformMC>()->SetIsActive(true);
	pCube->GetViewC<MeshVC>()->SetIsActive(true);
	pCube->SetIsActive(true);
}
