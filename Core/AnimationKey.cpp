#include "pch.h"
#include "AnimationKey.h"

using namespace Ion::Core;

AnimationKey::AnimationKey()
	: mTick{ 0.f }
	, mBoneTransforms{}
{
}

void AnimationKey::SetTick(float tick)
{
	mTick = tick;
}

const float AnimationKey::GetTick() const
{
	return mTick;
}

std::vector<DirectX::XMFLOAT4X4>& AnimationKey::GetBoneTransforms()
{
	return mBoneTransforms;
}
