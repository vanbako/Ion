#include "pch.h"
#include "ControlRMC.h"
#include "TransformMC.h"
#include "Object.h"
#include "Scene.h"
#include "NextObjectCmd.h"
#include "PrevObjectCmd.h"
#include "ShowControlsCmd.h"
#include "MoveObjectRMC.h"
#include "InputCC.h"
#include "KeyboardState.h"

using namespace Ion;

const std::string Core::ControlRMC::mName{ "Control" };

Core::ControlRMC::ControlRMC(bool isActive, Core::Object* pObject)
	: Core::ReceiverMC(isActive, pObject)
	, mCommands{
		{ "NextObject", new Core::NextObjectCmd{ this } },
		{ "PrevObject", new Core::PrevObjectCmd{ this } },
		{ "ShowControls", new Core::ShowControlsCmd{ this } } }
	, mpObjects{}
	, mCurrObject{ 0 }
	, mNextObject{ false, false }
	, mPrevObject{ false, false }
	, mShowControls{ false, false }
{
}

Core::ControlRMC::~ControlRMC()
{
	for (std::pair<std::string, Core::Command*>& command : mCommands)
		delete command.second;
}

void Core::ControlRMC::Initialize()
{
	mIsInitialized = true;
}

void Core::ControlRMC::Update(float delta)
{
	(delta);
	using namespace DirectX;
	if (!mIsActive)
		return;
	if (!mHasChanged)
		return;
	if (mpObjects.size() < 2)
		return;
	if (mNextObject[mCurrent])
	{
		size_t nextObject{ mCurrObject + 1 };
		if (nextObject == mpObjects.size())
			nextObject = 0;
		if (mpObjects[mCurrObject]->GetScene()->TryLockExclusiveControllerCs() &&
			mpObjects[mCurrObject]->HasModelC<MoveObjectRMC>() &&
			mpObjects[mCurrObject]->HasControllerC<InputCC>())
		{
			MoveObjectRMC* pMoveObjectRMC{ mpObjects[mCurrObject]->DetachModelC<MoveObjectRMC>() };
			mpObjects[nextObject]->AttachModelC<MoveObjectRMC>(pMoveObjectRMC);
			mpObjects[nextObject]->AttachControllerC<InputCC>(mpObjects[mCurrObject]->DetachControllerC<InputCC>());
			pMoveObjectRMC->SetTransformMC(mpObjects[nextObject]->GetModelC<TransformMC>());
			mpObjects[mCurrObject]->GetScene()->UnlockExclusiveControllerCs();
			mCurrObject = nextObject;
		}
	}
	if (mPrevObject[mCurrent])
	{
		size_t prevObject{ mpObjects.size() - 1 };
		if (mCurrObject != 0)
			prevObject = mCurrObject - 1;
		if (mpObjects[mCurrObject]->GetScene()->TryLockExclusiveControllerCs() &&
			mpObjects[mCurrObject]->HasModelC<MoveObjectRMC>() &&
			mpObjects[mCurrObject]->HasControllerC<InputCC>())
		{
			MoveObjectRMC* pMoveObjectRMC{ mpObjects[mCurrObject]->DetachModelC<MoveObjectRMC>() };
			mpObjects[prevObject]->AttachModelC<MoveObjectRMC>(pMoveObjectRMC);
			mpObjects[prevObject]->AttachControllerC<InputCC>(mpObjects[mCurrObject]->DetachControllerC<InputCC>());
			pMoveObjectRMC->SetTransformMC(mpObjects[prevObject]->GetModelC<TransformMC>());
			mpObjects[mCurrObject]->GetScene()->UnlockExclusiveControllerCs();
			mCurrObject = prevObject;
		}
	}
	if (mShowControls[mCurrent])
	{
	}
}

void Core::ControlRMC::Switch()
{
	if (!mIsActive)
		return;
	if (!mHasChanged)
		return;
	mHasChanged = false;
	int next{ 0 };
	if (mCurrent == 0)
		next = 1;
	mNextObject[next] = mNextObject[mCurrent];
	mPrevObject[next] = mPrevObject[mCurrent];
	mShowControls[next] = mShowControls[mCurrent];
	mNextObject[mCurrent] = false;
	mPrevObject[mCurrent] = false;
	mShowControls[mCurrent] = false;
}

const std::vector<std::pair<std::string, Core::Command*>>& Core::ControlRMC::GetCommands() const
{
	return mCommands;
}

const std::string& Core::ControlRMC::GetName() const
{
	return mName;
}

void Core::ControlRMC::AddObject(Core::Object* pObject)
{
	mpObjects.emplace_back(pObject);
}

void Core::ControlRMC::NextObject(long long value)
{
	if (KeyboardState(value) != KeyboardState::KeyDown)
		return;
	mNextObject[mCurrent] = true;
	mHasChanged = true;
}

void Core::ControlRMC::PrevObject(long long value)
{
	if (KeyboardState(value) != KeyboardState::KeyDown)
		return;
	mPrevObject[mCurrent] = true;
	mHasChanged = true;
}

void Core::ControlRMC::ShowControls(long long value)
{
	(value);
	mShowControls[mCurrent] = true;
	mHasChanged = true;
}
