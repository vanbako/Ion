#include "pch.h"
#include "SteeringRMC.h"
#include "TransformMC.h"
#include "Object.h"
#include "Scene.h"
#include "WanderCmd.h"

using namespace Ion;

const std::string Core::SteeringRMC::mName{ "Steering" };

Core::SteeringRMC::SteeringRMC(bool isActive, Core::Object* pObject)
	: Core::ReceiverMC(isActive, pObject)
	, mCommands{
		{ "Wander", new Core::WanderCmd{ this } } }
	, mpTransform{ nullptr }
	, mVelocity{ { 0.f, 0.f, -0.1f }, 0.f }
	, mOffset{ 6.f }
	, mRadius{ 4.f }
	, mAngleChange{ float(M_PI_4) * 0.5f }
	, mWanderAngle{ 0.f }
	, mWanderDeltas{}
{
}

Core::SteeringRMC::~SteeringRMC()
{
	for (std::pair<std::string, Core::Command*>& command : mCommands)
		delete command.second;
}

void Core::SteeringRMC::Initialize()
{
	mIsInitialized = true;
}

void Core::SteeringRMC::Update(float delta)
{
	(delta);
	using namespace DirectX;
	if (!mIsActive)
		return;
	if (mpTransform == nullptr)
	{
		mpTransform = mpObject->GetModelC<Core::TransformMC>();
		mHasChanged = true;
	}
	if (mpTransform == nullptr)
		return;
	if (!mHasChanged)
		return;

	for (auto& wanderDelta : mWanderDeltas)
		Steering(CalculateWander(wanderDelta), wanderDelta);
	mWanderDeltas.clear();
	mHasChanged = false;
}

void Core::SteeringRMC::Switch()
{
	//if (!mIsActive)
	//	return;
	//if (!mHasChanged)
	//	return;
}

const std::vector<std::pair<std::string, Core::Command*>>& Core::SteeringRMC::GetCommands() const
{
	return mCommands;
}

const std::string& Core::SteeringRMC::GetName() const
{
	return mName;
}

void Core::SteeringRMC::SetTransformMC(TransformMC* pTransform)
{
	mpTransform = pTransform;
}

void Core::SteeringRMC::Wander(float value)
{
	mWanderDeltas.emplace_back(value);
	mHasChanged = true;
}

float Core::SteeringRMC::GetOrientationFromVelocity(const Core::Velocity& velocity)
{
	if ((velocity.mLinear.x == 0) && ((velocity.mLinear.z == 0)))
		return 0.f;
	return std::atan2(-velocity.mLinear.x, -velocity.mLinear.z);
}

void Core::SteeringRMC::Steering(Core::Velocity velocity, float delta)
{
	using namespace DirectX;

	XMVECTOR vel{ XMLoadFloat3(&mVelocity.mLinear) };
	XMVECTOR force{ XMLoadFloat3(&velocity.mLinear) - vel };
	XMVECTOR acc{ force /* divive by mass */ };
	vel += acc * delta;
	XMStoreFloat3(&mVelocity.mLinear, vel);
	XMVECTOR pos{ XMLoadFloat4(&mpTransform->GetWorldPosition()) + vel };
	XMFLOAT4 newPos{};
	XMStoreFloat4(&newPos, pos);
	mpTransform->SetPosition(newPos);

	XMVECTOR rot{ XMQuaternionRotationAxis(XMVECTOR{ 0.f, 1.f, 0.f }, GetOrientationFromVelocity(mVelocity)) };
	XMFLOAT4 newRot{};
	XMStoreFloat4(&newRot, rot);
	mpTransform->SetRotation(newRot);
}

Core::Velocity Core::SteeringRMC::CalculateWander(float delta)
{
	(delta);
	using namespace DirectX;

	XMVECTOR offset{ mOffset * XMVector3Normalize(XMLoadFloat3(&mVelocity.mLinear)) };
	XMVECTOR circleOffset{ std::cos(mWanderAngle) * mRadius, 0.f, std::sin(mWanderAngle) * mRadius };
	mWanderAngle += mAngleChange * ((float(std::rand()) / float(RAND_MAX)) - .5f);
	XMVECTOR vel{ XMVector3Normalize(offset + circleOffset) * 0.1f }; // 0.1f = MaxLinearSpeed
	Core::Velocity velocity{};
	XMStoreFloat3(&velocity.mLinear, vel);
	return velocity;
}
