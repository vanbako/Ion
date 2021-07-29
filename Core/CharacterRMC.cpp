#include "pch.h"
#include "CharacterRMC.h"
#include "TransformMC.h"
#include "Object.h"
#include "MoveForwardCmd.h"
#include "MoveBackCmd.h"
#include "MoveLeftCmd.h"
#include "MoveRightCmd.h"
#include "RotateLeftCmd.h"
#include "RotateRightCmd.h"
#include "KeyboardState.h"

using namespace Ion;

const std::string Core::CharacterRMC::mName{ "Character" };

Core::CharacterRMC::CharacterRMC(bool isActive, Core::Object* pObject)
	: Core::ReceiverMC(isActive, pObject)
	, mCommands{
		{ "MoveForward", new Core::MoveForwardCmd{ this } },
		{ "MoveBack", new Core::MoveBackCmd{ this } },
		{ "MoveLeft", new Core::MoveLeftCmd{ this } },
		{ "MoveRight", new Core::MoveRightCmd{ this } },
		{ "RotateLeft", new Core::RotateLeftCmd{ this } },
		{ "RotateRight", new Core::RotateRightCmd{ this } } }
	, mpTransformMC{ pObject->GetModelC<Core::TransformMC>() }
	, mMoveLeft{ false }
{
}

Core::CharacterRMC::~CharacterRMC()
{
	for (std::pair<std::string, Core::Command*>& command : mCommands)
		delete command.second;
}

void Core::CharacterRMC::Initialize()
{
	if (mpTransformMC == nullptr)
		mpTransformMC = mpObject->GetModelC<Core::TransformMC>();
	mIsInitialized = true;
}

void Core::CharacterRMC::Update(float delta)
{
	(delta);
	if (!mIsActive)
		return;
	if (mpTransformMC == nullptr)
		return;
	if (!mHasChanged)
		return;
	//if (mMoveLeft)
	//	mpTransformMC->
}

void Core::CharacterRMC::Switch()
{
	if (!mIsActive)
		return;
	if (!mHasChanged)
		return;
	mHasChanged = false;
	int next{ 0 };
	if (mCurrent == 0)
		next = 1;
	mMoveForward[next] = mMoveForward[mCurrent];
	mMoveBack[next] = mMoveBack[mCurrent];
	mMoveLeft[next] = mMoveLeft[mCurrent];
	mMoveRight[next] = mMoveRight[mCurrent];
	mRotateLeft[next] = mRotateLeft[mCurrent];
	mRotateRight[next] = mRotateRight[mCurrent];
}

const std::vector<std::pair<std::string, Core::Command*>>& Core::CharacterRMC::GetCommands() const
{
	return mCommands;
}

const std::string& Core::CharacterRMC::GetName() const
{
	return mName;
}

void Core::CharacterRMC::MoveForward(long long value)
{
	(value);
	mMoveForward[mCurrent] = true;
	mHasChanged = true;
}

void Core::CharacterRMC::MoveBack(long long value)
{
	(value);
	mMoveBack[mCurrent] = true;
	mHasChanged = true;
}

void Core::CharacterRMC::MoveLeft(long long value)
{
	(value);
	mMoveLeft[mCurrent] = true;
	mHasChanged = true;
}

void Core::CharacterRMC::MoveRight(long long value)
{
	(value);
	mMoveRight[mCurrent] = true;
	mHasChanged = true;
}

void Core::CharacterRMC::RotateLeft(long long value)
{
	(value);
	mRotateLeft[mCurrent] = true;
	mHasChanged = true;
}

void Core::CharacterRMC::RotateRight(long long value)
{
	(value);
	mRotateRight[mCurrent] = true;
	mHasChanged = true;
}
