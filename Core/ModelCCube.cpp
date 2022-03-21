#include "pch.h"
#include "ModelCCube.h"
#include "Application.h"

using namespace Ion;

const float Core::ModelCCube::mModelUpdateDist{ 100000.f };

Core::ModelCCube::ModelCCube(const Core::Vector<long long>& location)
	: Cube(location)
	, mCubeModelCs{}
{
}

Core::ModelCCube::ModelCCube()
	: ModelCCube(Core::Vector<long long>{})
{
}

void Core::ModelCCube::SetHasMoved(Core::ModelC* pModelC, bool hasMoved)
{
	for (auto& cubeModelC : mCubeModelCs)
		if (cubeModelC.mpModelC == pModelC)
		{
			cubeModelC.mHasMoved = hasMoved;
			break;
		}
}

void Core::ModelCCube::AddModelC(bool hasMoved, Core::ModelC* pModelC)
{
	mCubeModelCs.emplace_back(hasMoved, pModelC);
}

void Core::ModelCCube::AddModelCCheckExistence(bool hasMoved, Core::ModelC* pModelC)
{
	auto it{ mCubeModelCs.begin() };
	while (it != mCubeModelCs.end())
	{
		if ((*it).mpModelC == pModelC)
		{
			(*it).mHasMoved = hasMoved;
			break;
		}
		++it;
	}
	if (it == mCubeModelCs.end())
		AddModelC(hasMoved, pModelC);
}

void Core::ModelCCube::ModelCUpdate(float delta)
{
	if ((mLocation.GetA() != LLONG_MAX) || (mLocation.GetB() != LLONG_MAX) || (mLocation.GetC() != LLONG_MAX))
	{
		for (Core::Canvas& canvas : mpScene->GetApplication()->GetCanvases())
			UpdateCanvasCamera(&canvas);
		bool ret{ true };
		for (CubeCanvasCamera& cubeCanvasCamera : mCubeCanvasCameras)
			if (cubeCanvasCamera.mDist <= mModelUpdateDist)
			{
				ret = false;
				break;
			}
		if (ret)
			return;
	}
	for (auto& cubeModelC : mCubeModelCs)
		if (!cubeModelC.mHasMoved)
			cubeModelC.mpModelC->Update(delta);
}

void Core::ModelCCube::ModelCSwitch()
{
	if ((mLocation.GetA() != LLONG_MAX) || (mLocation.GetB() != LLONG_MAX) || (mLocation.GetC() != LLONG_MAX))
	{
		bool ret{ true };
		for (CubeCanvasCamera& cubeCanvasCamera : mCubeCanvasCameras)
			if (cubeCanvasCamera.mDist <= mModelUpdateDist)
			{
				ret = false;
				break;
			}
		if (ret)
			return;
	}
	for (auto& cubeModelC : mCubeModelCs)
		if (!cubeModelC.mHasMoved)
			cubeModelC.mpModelC->Switch();
}
