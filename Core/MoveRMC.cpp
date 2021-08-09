#include "pch.h"
#include "MoveRMC.h"

using namespace Ion;

Core::MoveRMC::MoveRMC(bool isActive, Core::Object* pObject)
	: Core::ReceiverMC(isActive, pObject)
	, mMoveForward{ false, false }
	, mMoveBack{ false, false }
	, mMoveLeft{ false, false }
	, mMoveRight{ false, false }
	, mMoveUp{ false, false }
	, mMoveDown{ false, false }
	, mRotateLeft{ false, false }
	, mRotateRight{ false, false }
{
}

void Core::MoveRMC::Switch()
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
	mMoveUp[next] = mMoveUp[mCurrent];
	mMoveDown[next] = mMoveDown[mCurrent];
	mRotateLeft[next] = mRotateLeft[mCurrent];
	mRotateRight[next] = mRotateRight[mCurrent];
	if (mMoveForward[mCurrent] || mMoveBack[mCurrent] || mMoveLeft[mCurrent] || mMoveRight[mCurrent] || mMoveUp[mCurrent] || mMoveDown[mCurrent] || mRotateLeft[mCurrent] || mRotateRight[mCurrent])
		mHasChanged = true;
}

void Core::MoveRMC::MoveForward(long long value)
{
	if (KeyboardState(value) == KeyboardState::KeyDown)
	{
		mMoveForward[mCurrent] = true;
		mHasChanged = true;
	}
	else if (KeyboardState(value) == KeyboardState::KeyUp)
		mMoveForward[mCurrent] = false;
}

void Core::MoveRMC::MoveBack(long long value)
{
	if (KeyboardState(value) == KeyboardState::KeyDown)
	{
		mMoveBack[mCurrent] = true;
		mHasChanged = true;
	}
	else if (KeyboardState(value) == KeyboardState::KeyUp)
		mMoveBack[mCurrent] = false;
}

void Core::MoveRMC::MoveLeft(long long value)
{
	if (KeyboardState(value) == KeyboardState::KeyDown)
	{
		mMoveLeft[mCurrent] = true;
		mHasChanged = true;
	}
	else if (KeyboardState(value) == KeyboardState::KeyUp)
		mMoveLeft[mCurrent] = false;
}

void Core::MoveRMC::MoveRight(long long value)
{
	if (KeyboardState(value) == KeyboardState::KeyDown)
	{
		mMoveRight[mCurrent] = true;
		mHasChanged = true;
	}
	else if (KeyboardState(value) == KeyboardState::KeyUp)
		mMoveRight[mCurrent] = false;
}

void Core::MoveRMC::MoveUp(long long value)
{
	if (KeyboardState(value) == KeyboardState::KeyDown)
	{
		mMoveUp[mCurrent] = true;
		mHasChanged = true;
	}
	else if (KeyboardState(value) == KeyboardState::KeyUp)
		mMoveUp[mCurrent] = false;
}

void Core::MoveRMC::MoveDown(long long value)
{
	if (KeyboardState(value) == KeyboardState::KeyDown)
	{
		mMoveDown[mCurrent] = true;
		mHasChanged = true;
	}
	else if (KeyboardState(value) == KeyboardState::KeyUp)
		mMoveDown[mCurrent] = false;
}

void Core::MoveRMC::RotateLeft(long long value)
{
	if (KeyboardState(value) == KeyboardState::KeyDown)
	{
		mRotateLeft[mCurrent] = true;
		mHasChanged = true;
	}
	else if (KeyboardState(value) == KeyboardState::KeyUp)
		mRotateLeft[mCurrent] = false;
}

void Core::MoveRMC::RotateRight(long long value)
{
	if (KeyboardState(value) == KeyboardState::KeyDown)
	{
		mRotateRight[mCurrent] = true;
		mHasChanged = true;
	}
	else if (KeyboardState(value) == KeyboardState::KeyUp)
		mRotateRight[mCurrent] = false;
}
