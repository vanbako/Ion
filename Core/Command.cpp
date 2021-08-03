#include "pch.h"
#include "Command.h"
#include "ReceiverMC.h"

using namespace Ion;

Core::Command::Command()
	: mBoolValue{ false }
	, mIntValue{ 0 }
	, mLongLongValue{ 0 }
	, mFloatValue{ 0.f }
{
}

void Core::Command::SetValue(bool value)
{
	mBoolValue = value;
}

void Core::Command::SetValue(int value)
{
	mIntValue = value;
}

void Core::Command::SetValue(long long value)
{
	mLongLongValue = value;
}

void Core::Command::SetValue(float value)
{
	mFloatValue = value;
}
