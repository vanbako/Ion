#include "pch.h"
#include "ControllerCCube.h"
#include "Application.h"

using namespace Ion;

const float Core::ControllerCCube::mControllerUpdateDist{ 100000.f };

Core::ControllerCCube::ControllerCCube(const Core::Vector<long long>& location)
	: Cube(location)
	, mCubeControllerCs{}
{
}

Core::ControllerCCube::ControllerCCube()
	: ControllerCCube(Core::Vector<long long>{})
{
}

void Core::ControllerCCube::SetHasMoved(Core::ControllerC* pControllerC, bool hasMoved)
{
	for (auto& cubeControllerC : mCubeControllerCs)
		if (cubeControllerC.mpControllerC == pControllerC)
		{
			cubeControllerC.mHasMoved = hasMoved;
			break;
		}
}

void Core::ControllerCCube::AddControllerC(bool hasMoved, Core::ControllerC* pControllerC)
{
	mCubeControllerCs.emplace_back(hasMoved, pControllerC);
}

void Core::ControllerCCube::AddControllerCCheckExistence(bool hasMoved, Core::ControllerC* pControllerC)
{
	auto it{ mCubeControllerCs.begin() };
	while (it != mCubeControllerCs.end())
	{
		if ((*it).mpControllerC == pControllerC)
		{
			(*it).mHasMoved = hasMoved;
			break;
		}
		++it;
	}
	if (it == mCubeControllerCs.end())
		AddControllerC(hasMoved, pControllerC);
}

void Core::ControllerCCube::ControllerCUpdate(float delta)
{
	if ((mLocation.GetA() != LLONG_MAX) || (mLocation.GetB() != LLONG_MAX) || (mLocation.GetC() != LLONG_MAX))
	{
		for (Core::Canvas& canvas : mpScene->GetApplication()->GetCanvases())
			UpdateCanvasCamera(&canvas);
		bool ret{ true };
		for (CubeCanvasCamera& cubeCanvasCamera : mCubeCanvasCameras)
			if (cubeCanvasCamera.mDist <= mControllerUpdateDist)
			{
				ret = false;
				break;
			}
		if (ret)
			return;
	}
	for (auto& cubeControllerC : mCubeControllerCs)
		if (!cubeControllerC.mHasMoved)
			cubeControllerC.mpControllerC->Update(delta);
}
