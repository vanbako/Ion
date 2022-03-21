#include "pch.h"
#include "../Test/include/ControlZeroRMC.h"
#include "../Core/include/TransformMC.h"
#include "../Core/include/Application.h"
#include "../Core/include/Object.h"
#include "../Core/include/Scene.h"
#include "../Test/include/ToggleSceneOneCmd.h"
#include "../Test/include/ToggleSceneTwoCmd.h"
#include "../Core/include/InputCC.h"
#include "../Core/include/KeyboardState.h"

using namespace Ion;

const std::string Core::ControlZeroRMC::mName{ "ControlZero" };

Core::ControlZeroRMC::ControlZeroRMC(bool isActive, Core::Object* pObject)
	: Core::ReceiverMC(isActive, pObject)
	, mCommands{
		{ "ToggleSceneOne", new Core::ToggleSceneOneCmd{ this } },
		{ "ToggleSceneTwo", new Core::ToggleSceneTwoCmd{ this } } }
	, mToggleSceneOne{ false, false }
	, mToggleSceneTwo{ false, false }
{
}

Core::ControlZeroRMC::~ControlZeroRMC()
{
	for (std::pair<std::string, Core::Command*>& command : mCommands)
		delete command.second;
}

void Core::ControlZeroRMC::Initialize()
{
	Core::ModelC::Initialize();
}

void Core::ControlZeroRMC::Update(float delta)
{
	using namespace DirectX;
	if (!mIsActive)
		return;
	Core::ModelC::Update(delta);
	if (!mHasChanged)
		return;
	if (mToggleSceneOne[mCurrent])
	{
		Core::Scene* pScene{ mpObject->GetScene()->GetApplication()->GetScene(1) };
		pScene->SetIsActive(!pScene->GetIsActive());
	}
	if (mToggleSceneTwo[mCurrent])
	{
		Core::Scene* pScene{ mpObject->GetScene()->GetApplication()->GetScene(2) };
		pScene->SetIsActive(!pScene->GetIsActive());
	}
}

void Core::ControlZeroRMC::Switch()
{
	if (!mIsActive)
		return;
	if (!mHasChanged)
		return;
	mHasChanged = false;
	int next{ 0 };
	if (mCurrent == 0)
		next = 1;
	mToggleSceneOne[next] = mToggleSceneOne[mCurrent];
	mToggleSceneTwo[next] = mToggleSceneTwo[mCurrent];
	mToggleSceneOne[mCurrent] = false;
	mToggleSceneTwo[mCurrent] = false;
}

const std::vector<std::pair<std::string, Core::Command*>>& Core::ControlZeroRMC::GetCommands() const
{
	return mCommands;
}

const std::string& Core::ControlZeroRMC::GetName() const
{
	return mName;
}

void Core::ControlZeroRMC::ToggleSceneOne(long long value)
{
	if (KeyboardState(value) != KeyboardState::KeyDown)
		return;
	mToggleSceneOne[mCurrent] = true;
	mHasChanged = true;
}

void Core::ControlZeroRMC::ToggleSceneTwo(long long value)
{
	if (KeyboardState(value) != KeyboardState::KeyDown)
		return;
	mToggleSceneTwo[mCurrent] = true;
	mHasChanged = true;
}
