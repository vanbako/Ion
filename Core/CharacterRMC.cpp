#include "../Core/pch.h"
#include "../Core/CharacterRMC.h"
#include "../Core/TransformMC.h"
#include "../Core/Object.h"
#include "../Core/MoveForwardCmd.h"
#include "../Core/MoveBackCmd.h"
#include "../Core/MoveLeftCmd.h"
#include "../Core/MoveRightCmd.h"
#include "../Core/RotateLeftCmd.h"
#include "../Core/RotateRightCmd.h"

using namespace Ion::Core;

const std::string CharacterRMC::mName{ "Character" };

CharacterRMC::CharacterRMC(bool isActive, Object* pObject)
	: ReceiverMC(isActive, pObject)
	, mCommands{
		{ "MoveForward", new MoveForwardCmd{ this } },
		{ "MoveBack", new MoveBackCmd{ this } },
		{ "MoveLeft", new MoveLeftCmd{ this } },
		{ "MoveRight", new MoveRightCmd{ this } },
		{ "RotateLeft", new RotateLeftCmd{ this } },
		{ "RotateRight", new RotateRightCmd{ this } } }
	, mpTransformMC{ pObject->GetModelC<TransformMC>() }
	, mMoveLeft{ false }
{
}

CharacterRMC::~CharacterRMC()
{
	for (std::pair<std::string, Command*>& command : mCommands)
		delete command.second;
}

void CharacterRMC::Initialize()
{
	if (mpTransformMC == nullptr)
		mpTransformMC = mpObject->GetModelC<TransformMC>();
	mIsInitialized = true;
}

void CharacterRMC::Update(float delta)
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

void CharacterRMC::Switch()
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

const std::vector<std::pair<std::string, Command*>>& Ion::Core::CharacterRMC::GetCommands() const
{
	return mCommands;
}

const std::string& Ion::Core::CharacterRMC::GetName() const
{
	return mName;
}

void CharacterRMC::MoveForward()
{
	mMoveForward[mCurrent] = true;
	mHasChanged = true;
}

void CharacterRMC::MoveBack()
{
	mMoveBack[mCurrent] = true;
	mHasChanged = true;
}

void CharacterRMC::MoveLeft()
{
	mMoveLeft[mCurrent] = true;
	mHasChanged = true;
}

void CharacterRMC::MoveRight()
{
	mMoveRight[mCurrent] = true;
	mHasChanged = true;
}

void CharacterRMC::RotateLeft()
{
	mRotateLeft[mCurrent] = true;
	mHasChanged = true;
}

void CharacterRMC::RotateRight()
{
	mRotateRight[mCurrent] = true;
	mHasChanged = true;
}
