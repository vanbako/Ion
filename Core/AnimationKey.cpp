#include "../Core/pch.h"
#include "../Core/AnimationKey.h"

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

std::vector<DirectX::XMFLOAT4X4>& AnimationKey::GetBoneTransforms()
{
	return mBoneTransforms;
}
