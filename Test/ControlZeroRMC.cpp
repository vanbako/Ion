#include "pch.h"
#include "../Test/include/ControlZeroRMC.h"
#include "../Core/include/TransformMC.h"
#include "../Core/include/Application.h"
#include "../Core/include/Object.h"
#include "../Core/include/Scene.h"
#include "../Test/include/ToggleSceneOneCmd.h"
#include "../Test/include/ToggleSceneTwoCmd.h"
#include "../Test/include/ToggleRelativeCursorCmd.h"
#include "../Core/include/InputCC.h"
#include "../Core/include/KeyboardState.h"

using namespace Ion;

const std::string Core::ControlZeroRMC::mName{ "ControlZero" };

Core::ControlZeroRMC::ControlZeroRMC(bool isActive, Core::Object* pObject)
	: Core::ReceiverMC(isActive, pObject)
	, mCommands{
		{ "ToggleSceneOne", new Core::ToggleSceneOneCmd{ this } },
		{ "ToggleSceneTwo", new Core::ToggleSceneTwoCmd{ this } },
		{ "ToggleRelativeCursor", new Core::ToggleRelativeCursorCmd{ this } } }
	, mToggleSceneOne{ false }
	, mToggleSceneTwo{ false }
	, mCursorModeRelative{ false }
{
	if (mpObject->GetScene()->GetApplication()->GetCursorMode() == Core::CursorMode::Relative)
		mCursorModeRelative = true;
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
	if (mToggleSceneOne)
	{
		Core::Scene* pScene{ mpObject->GetScene()->GetApplication()->GetScene(1) };
		pScene->SetIsActive(!pScene->GetIsActive());
	}
	if (mToggleSceneTwo)
	{
		Core::Scene* pScene{ mpObject->GetScene()->GetApplication()->GetScene(2) };
		pScene->SetIsActive(!pScene->GetIsActive());
	}
}

void Core::ControlZeroRMC::Switch()
{
	if (!mIsActive)
		return;
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
	mToggleSceneOne = true;
}

void Core::ControlZeroRMC::ToggleSceneTwo(long long value)
{
	if (KeyboardState(value) != KeyboardState::KeyDown)
		return;
	mToggleSceneTwo = true;
}

void Core::ControlZeroRMC::ToggleCursorMode(long long value)
{
	if (KeyboardState(value) == KeyboardState::KeyDown)
	{
		if (!mCursorModeRelative)
		{
			mCursorModeRelative = true;
			mpObject->GetScene()->GetApplication()->SetCursorMode(Core::CursorMode::Relative);
		}
	}
	else
		if (mCursorModeRelative)
		{
			mCursorModeRelative = false;
			mpObject->GetScene()->GetApplication()->SetCursorMode(Core::CursorMode::Absolute);
		}
}
