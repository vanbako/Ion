#include "pch.h"
#include "MoveRMC.h"

using namespace Ion;

const std::chrono::milliseconds Core::MoveRMC::mMoveActionsMutexDuration{ 1 };

Core::MoveRMC::MoveRMC(bool isActive, Core::Object* pObject)
	: Core::ReceiverMC(isActive, pObject)
	, mMoveBool{ false }
	, mMoveActionsMutex{}
	, mMoveActions{}
{
}

void Core::MoveRMC::Switch()
{
	if (!mIsActive)
		return;
}

void Core::MoveRMC::MoveForward(long long value)
{
	SetMoveState(Core::MoveType::Forward, value);
}

void Core::MoveRMC::MoveBack(long long value)
{
	SetMoveState(Core::MoveType::Back, value);
}

void Core::MoveRMC::MoveLeft(long long value)
{
	SetMoveState(Core::MoveType::Left, value);
}

void Core::MoveRMC::MoveRight(long long value)
{
	SetMoveState(Core::MoveType::Right, value);
}

void Core::MoveRMC::MoveUp(long long value)
{
	SetMoveState(Core::MoveType::Up, value);
}

void Core::MoveRMC::MoveDown(long long value)
{
	SetMoveState(Core::MoveType::Down, value);
}

void Core::MoveRMC::RotateLeft(long long value)
{
	SetMoveState(Core::MoveType::RotateLeft, value);
}

void Core::MoveRMC::RotateRight(long long value)
{
	SetMoveState(Core::MoveType::RotateRight, value);
}

void Core::MoveRMC::CursorRotateLeftRight(int value)
{
	AddMoveAction(Core::MoveType::CursorRotateLeftRight, value);
}

void Core::MoveRMC::CursorUpDown(int value)
{
	AddMoveAction(Core::MoveType::CursorUpDown, value);
}

void Core::MoveRMC::SetMoveState(Core::MoveType moveType, long long value)
{
	if (KeyboardState(value) == KeyboardState::KeyDown)
		mMoveBool[std::size_t(moveType)] = true;
	else if (KeyboardState(value) == KeyboardState::KeyUp)
		mMoveBool[std::size_t(moveType)] = false;
}

void Core::MoveRMC::AddMoveAction(Core::MoveType moveType, int value)
{
	if (!mMoveActionsMutex.try_lock_for(mMoveActionsMutexDuration))
		return;
	mMoveActions.emplace_back(moveType, value);
	mMoveActionsMutex.unlock();
}
