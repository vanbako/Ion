#include "pch.h"
#include "AnimationClip.h"

using namespace Ion;

Core::AnimationClip::AnimationClip()
	: mName{}
	, mDuration{ 0.f }
	, mTicksPerSecond{ 0.f }
	, mKeys{}
{
}

void Core::AnimationClip::SetName(const std::wstring& name)
{
	mName = name;
}

void Core::AnimationClip::SetDuration(float duration)
{
	mDuration = duration;
}

void Core::AnimationClip::SetTicksPerSecond(float ticksPerSecond)
{
	mTicksPerSecond = ticksPerSecond;
}

const float Core::AnimationClip::GetDuration() const
{
	return mDuration;
}

const float Core::AnimationClip::GetTicksPerSecond() const
{
	return mTicksPerSecond;
}

std::vector<Core::AnimationKey>& Core::AnimationClip::GetKeys()
{
	return mKeys;
}
