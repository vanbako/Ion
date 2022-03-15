#include "pch.h"
#include "Cube.h"
#include "ViewC.h"
#include "Canvas.h"
#include "Object.h"

using namespace Ion;

Core::Cube::Cube(Core::Material3D* pMaterial, Core::Vector<long long> location)
	: mpMaterial{ pMaterial }
	, mLocation{ location }
	, mCubeViewCs{}
{
}

Core::Cube::Cube(Core::Material3D* pMaterial)
	: mpMaterial{ pMaterial }
	, mLocation{}
	, mCubeViewCs{}
{
}

void Core::Cube::SetHasMoved(Core::ViewC* pViewC, bool hasMoved)
{
	for (auto& cubeViewC : mCubeViewCs)
		if (cubeViewC.mpViewC == pViewC)
		{
			cubeViewC.mHasMoved = hasMoved;
			break;
		}
}

void Core::Cube::AddViewC(bool hasMoved, Core::ViewC* pViewC)
{
	mCubeViewCs.emplace_back(hasMoved, pViewC);
}

void Core::Cube::AddViewCCheckExistence(bool hasMoved, Core::ViewC* pViewC)
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

Core::Vector<long long>& Core::Cube::GetLocation()
{
	return mLocation;
}

void Core::Cube::SetLocation(const Core::Vector<long long>& loc)
{
	mLocation = loc;
}

void Core::Cube::Render(Canvas* pCanvas, Material3D* pMaterial)
{
	using namespace DirectX;
	TransformMC* pTransform{ pCanvas->GetCamera()->GetModelC<TransformMC>() };
	XMVECTOR camForward{ XMLoadFloat4(&pTransform->GetForward()) };
	camForward = XMVector3Normalize(camForward);
	XMVECTOR camPos{ XMLoadFloat4(&pTransform->GetWorldPosition()) };
	XMFLOAT3 cubePosFloat3{ float(mLocation.GetA()), float(mLocation.GetB()), float(mLocation.GetC()) };
	XMVECTOR cubePos{ XMLoadFloat3(&cubePosFloat3) };
	XMVECTOR cubeDir{ cubePos - camPos };
	XMVECTOR cubeDist{ XMVector3LengthSq(cubeDir) };
	cubeDir = XMVector3Normalize(cubeDir);
	XMVECTOR dotProd{ XMVector3Dot(cubeDir, camForward) };
	float
		dot{},
		distSq{};
	XMStoreFloat(&dot, dotProd);
	XMStoreFloat(&distSq, cubeDist);
	// TODO: Magic number -> static member var
	if ((dot < 0.f) && (distSq > 1000000.f))
		return;
	for (auto& cubeViewC : mCubeViewCs)
		if (!cubeViewC.mHasMoved)
			cubeViewC.mpViewC->Render(pCanvas, pMaterial, distSq);
}
