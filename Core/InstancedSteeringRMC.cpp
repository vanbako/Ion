#include "pch.h"
#include "InstancedSteeringRMC.h"
#include "InstancedTransformMC.h"
#include "Object.h"
#include "Scene.h"
#include "WanderCmd.h"

using namespace Ion;

Core::InstancedSteeringRMC::InstancedSteeringRMC(bool isActive, Core::Object* pObject)
	: Core::SteeringRMC(isActive, pObject)
	, mpInstancedTransform{ nullptr }
	, mpTargets{}
	, mVelocities{}
	, mWanderData{}
{
}

void Core::InstancedSteeringRMC::Initialize()
{
	mIsInitialized = true;
}

void Core::InstancedSteeringRMC::Update(float delta)
{
	(delta);
	using namespace DirectX;
	if (!mIsActive)
		return;
	if (mpInstancedTransform == nullptr)
	{
		SetInstancedTransformMC(mpObject->GetModelC<Core::InstancedTransformMC>());
		mHasChanged = true;
	}
	if (mpInstancedTransform == nullptr)
		return;
	if (!mHasChanged)
		return;

	for (auto& wanderDelta : mWanderDeltas)
	{
		auto velocityIt{ mVelocities.begin() };
		auto wanderIt{ mWanderData.begin() };
		Core::WanderData wanderData{ 6.f, 4.f, float(M_PI_4) * 0.5f, 0.f };
		for (auto& transform : mpInstancedTransform->GetInstances())
		{
			Steering(&transform, *velocityIt, CalculateWander(*wanderIt, *velocityIt), wanderDelta);
			++velocityIt;
			++wanderIt;
		}
	}
	mWanderDeltas.clear();
	for (auto& seekDelta : mSeekDeltas)
	{
		auto velocityIt{ mVelocities.begin() };
		auto targetIt{ mpTargets.begin() };
		for (auto& transform : mpInstancedTransform->GetInstances())
		{
			if (*targetIt != nullptr)
				Steering(&transform, *velocityIt, CalculateSeek(&transform, *targetIt), seekDelta);
			++velocityIt;
			++targetIt;
		}
	}
	mSeekDeltas.clear();
	mHasChanged = false;
}

void Core::InstancedSteeringRMC::Switch()
{
	//if (!mIsActive)
	//	return;
	//if (!mHasChanged)
	//	return;
}

void Core::InstancedSteeringRMC::SetInstancedTransformMC(InstancedTransformMC* pInstancedTransform)
{
	mpInstancedTransform = pInstancedTransform;
	mVelocities.clear();
	mWanderData.clear();
	mpTargets.clear();
	Core::Velocity velocity{ { 0.f, 0.f, -0.1f }, 0.f };
	Core::WanderData wanderData{ 6.f, 4.f, float(M_PI_4) * 0.5f, 0.f };
	for (auto& transform : mpInstancedTransform->GetInstances())
	{
		(transform);
		mVelocities.emplace_back(velocity);
		mWanderData.emplace_back(wanderData);
		mpTargets.emplace_back(nullptr);
	}
}

void Core::InstancedSteeringRMC::SetTarget(std::size_t index, TransformMC* pTarget)
{
	if (mpTargets.size() > index)
		mpTargets[index] = pTarget;
}
