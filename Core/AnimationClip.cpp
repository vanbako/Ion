#include "../Core/pch.h"
#include "../Core/AnimationClip.h"

using namespace Ion::Core;

AnimationClip::AnimationClip()
	: mName{}
	, mDuration{ 0.f }
	, mTicksPerSecond{ 0.f }
	, mKeys{}
{
}

void AnimationClip::SetName(const std::wstring& name)
{
	mName = name;
}

void AnimationClip::SetDuration(float duration)
{
	mDuration = duration;
}

void AnimationClip::SetTicksPerSecond(float ticksPerSecond)
{
	mTicksPerSecond = ticksPerSecond;
}

std::vector<AnimationKey>& AnimationClip::GetKeys()
{
	return mKeys;
}
