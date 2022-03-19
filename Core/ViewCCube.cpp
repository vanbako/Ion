#include "pch.h"
#include "ViewCCube.h"
#include "ViewC.h"
#include "Canvas.h"
#include "Object.h"

using namespace Ion;

const float Core::ViewCCube::mBackRenderDist{ 1000.f };

Core::ViewCCube::ViewCCube(const Core::Vector<long long>& location)
	: Cube(location)
	, mpMaterial3D{ nullptr }
	, mCubeViewCs{}
{
}

Core::ViewCCube::ViewCCube()
	: ViewCCube(Core::Vector<long long>{})
{
}

void Ion::Core::ViewCCube::SetMaterial3D(Core::Material3D* pMaterial3D)
{
	mpMaterial3D = pMaterial3D;
}

Core::Material3D* Ion::Core::ViewCCube::GetMaterial3D()
{
	return mpMaterial3D;
}

void Core::ViewCCube::SetHasMoved(Core::ViewC* pViewC, bool hasMoved)
{
	for (auto& cubeViewC : mCubeViewCs)
		if (cubeViewC.mpViewC == pViewC)
		{
			cubeViewC.mHasMoved = hasMoved;
			break;
		}
}

void Core::ViewCCube::AddViewC(bool hasMoved, Core::ViewC* pViewC)
{
	mCubeViewCs.emplace_back(hasMoved, pViewC);
}

void Core::ViewCCube::AddViewCCheckExistence(bool hasMoved, Core::ViewC* pViewC)
{
	auto it{ mCubeViewCs.begin() };
	while (it != mCubeViewCs.end())
		if ((*it).mpViewC == pViewC)
		{
			(*it).mHasMoved = hasMoved;
			break;
		}
	if (it == mCubeViewCs.end())
		AddViewC(hasMoved, pViewC);
}

void Core::ViewCCube::Render(Canvas* pCanvas, Material3D* pMaterial)
{
	CanvasCamera* pCanvasCamera{ GetCanvasCamera(pCanvas) };
	if ((pCanvasCamera->mDot < 0.f) && (pCanvasCamera->mDist > mBackRenderDist))
		return;
	for (auto& cubeViewC : mCubeViewCs)
		if (!cubeViewC.mHasMoved)
			cubeViewC.mpViewC->Render(pCanvas, pMaterial, pCanvasCamera->mDist);
}

void Core::ViewCCube::ViewCUpdate(Canvas* pCanvas, float delta)
{
	CanvasCamera* pCanvasCamera{ GetCanvasCamera(pCanvas) };
	if ((pCanvasCamera->mDot < 0.f) && (pCanvasCamera->mDist > mBackRenderDist))
		return;
	for (auto& cubeViewC : mCubeViewCs)
		if (!cubeViewC.mHasMoved)
			cubeViewC.mpViewC->Update(delta);
}

Core::CanvasCamera* Core::ViewCCube::GetCanvasCamera(Canvas* pCanvas)
{
	TransformMC* pTransform{ pCanvas->GetCamera()->GetModelC<TransformMC>() };
	CanvasCamera* pCanvasCamera{ nullptr };
	for (auto it{ mCanvasCameras.begin() }; it != mCanvasCameras.end(); ++it)
		if ((*it).mpCanvas == pCanvas)
		{
			pCanvasCamera = &(*it);
			break;
		}
	bool calc{ true };
	if (pCanvasCamera == nullptr)
		pCanvasCamera = &mCanvasCameras.emplace_back(pCanvas, 0.f, 0.f);
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
	return pCanvasCamera;
}
