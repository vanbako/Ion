#include "pch.h"
#include "ObjectSteeringRMC.h"
#include "TransformMC.h"
#include "Object.h"
#include "Scene.h"
#include "WanderCmd.h"

using namespace Ion;

Core::ObjectSteeringRMC::ObjectSteeringRMC(bool isActive, Core::Object* pObject)
	: Core::SteeringRMC(isActive, pObject)
	, mpTransform{ nullptr }
	, mpTarget{ nullptr }
	, mVelocity{ { 0.f, 0.f, -0.1f }, 0.f }
	, mWander{ 6.f, 4.f, float(M_PI_4) * 0.5f, 0.f }
{
}

void Core::ObjectSteeringRMC::Initialize()
{
	Core::ModelC::Initialize();
}

void Core::ObjectSteeringRMC::Update(float delta)
{
	(delta);
	using namespace DirectX;
	if (!mIsActive)
		return;
	Core::ModelC::Update(delta);
	if (mpTransform == nullptr)
	{
		SetTransformMC(mpObject->GetModelC<Core::TransformMC>());
		mHasChanged = true;
	}
	if (mpTransform == nullptr)
		return;
	if (!mHasChanged)
		return;

	for (auto& wanderDelta : mWanderDeltas)
		Steering(mpTransform, mVelocity, CalculateWander(mWander, mVelocity), wanderDelta);
	mWanderDeltas.clear();
	if (mpTarget != nullptr)
		for (auto& seekDelta : mSeekDeltas)
			Steering(mpTransform, mVelocity, CalculateSeek(mpTransform, mpTarget), seekDelta);
	mSeekDeltas.clear();
	mHasChanged = false;
}

void Core::ObjectSteeringRMC::Switch()
{
	//if (!mIsActive)
	//	return;
	//if (!mHasChanged)
	//	return;
}

void Core::ObjectSteeringRMC::SetTransformMC(TransformMC* pTransform)
{
	mpTransform = pTransform;
}

void Core::ObjectSteeringRMC::SetTarget(TransformMC* pTarget)
{
	mpTarget = pTarget;
}
