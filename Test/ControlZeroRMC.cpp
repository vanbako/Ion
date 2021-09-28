#include "pch.h"
#include "ControlZeroRMC.h"
#include "TransformMC.h"
#include "Application.h"
#include "Object.h"
#include "Scene.h"
#include "ToggleSceneOneCmd.h"
#include "ToggleSceneTwoCmd.h"
#include "InputCC.h"
#include "KeyboardState.h"

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
	mIsInitialized = true;
}

void Core::ControlZeroRMC::Update(float delta)
{
	(delta);
	using namespace DirectX;
	if (!mIsActive)
		return;
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
