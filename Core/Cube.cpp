#include "pch.h"
#include "Cube.h"
#include "Canvas.h"
#include "Object.h"
#include "Scene.h"

using namespace Ion;

Core::Cube::Cube(const Core::Vector<long long>& location)
	: mLocation{ location }
	, mCubeCanvasCameras{}
	, mpScene{ nullptr }
{
}

Core::Cube::Cube()
	: Cube(Core::Vector<long long>{})
{
}

void Core::Cube::SetScene(Core::Scene* pScene)
{
	mpScene = pScene;
}

Core::Scene* Core::Cube::GetScene()
{
	return mpScene;
}

Core::Vector<long long>& Core::Cube::GetLocation()
{
	return mLocation;
}

void Core::Cube::SetLocation(const Core::Vector<long long>& loc)
{
	mLocation = loc;
}

void Core::Cube::UpdateCanvasCamera(Core::Canvas* pCanvas)
{
	Core::Object* pCamera{ pCanvas->GetCamera() };
	if (pCamera == nullptr)
		return;
	Core::TransformMC* pTransform{ pCanvas->GetCamera()->GetModelC<TransformMC>() };
	Core::CubeCanvasCamera* pCanvasCamera{ nullptr };
	for (auto it{ mCubeCanvasCameras.begin() }; it != mCubeCanvasCameras.end(); ++it)
		if ((*it).mpCanvas == pCanvas)
		{
			pCanvasCamera = &(*it);
			break;
		}
	bool calc{ true };
	if (pCanvasCamera == nullptr)
		pCanvasCamera = &mCubeCanvasCameras.emplace_back(pCanvas, 0.f, 0.f);
	else
		if (!pTransform->GetHasMoved())
			calc = false;
	if (calc)
	{
		using namespace DirectX;
		XMVECTOR camForward{ XMLoadFloat4(&pTransform->GetForward()) };
		camForward = XMVector3Normalize(camForward);
		XMVECTOR camPos{ XMLoadFloat4(&pTransform->GetWorldPosition()) };
		XMFLOAT3 cubePosFloat3{ float(mLocation.GetA()), float(mLocation.GetB()), float(mLocation.GetC()) };
		XMVECTOR cubePos{ XMLoadFloat3(&cubePosFloat3) };
		XMVECTOR cubeDir{ cubePos - camPos };
		XMVECTOR cubeDist{ XMVector3Length(cubeDir) };
		cubeDir = XMVector3Normalize(cubeDir);
		XMVECTOR dotProd{ XMVector3Dot(cubeDir, camForward) };
		XMStoreFloat(&pCanvasCamera->mDot, dotProd);
		XMStoreFloat(&pCanvasCamera->mDist, cubeDist);
	}
}
