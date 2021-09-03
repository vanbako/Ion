#include "pch.h"
#include "SteeringRMC.h"
#include "TransformMC.h"
#include "Object.h"
#include "Scene.h"
#include "WanderCmd.h"
#include "SeekCmd.h"

using namespace Ion;

const std::string Core::SteeringRMC::mName{ "Steering" };
const float Core::SteeringRMC::mMaxSpeed{ 0.1f };

Core::SteeringRMC::SteeringRMC(bool isActive, Core::Object* pObject)
	: Core::ReceiverMC(isActive, pObject)
	, mCommands{
		{ "Wander", new Core::WanderCmd{ this } },
		{ "Seek", new Core::SeekCmd{ this } } }
	, mWanderDeltas{}
	, mSeekDeltas{}
{
}

Core::SteeringRMC::~SteeringRMC()
{
	for (std::pair<std::string, Core::Command*>& command : mCommands)
		delete command.second;
}

const std::vector<std::pair<std::string, Core::Command*>>& Core::SteeringRMC::GetCommands() const
{
	return mCommands;
}

const std::string& Core::SteeringRMC::GetName() const
{
	return mName;
}

void Core::SteeringRMC::Wander(float value)
{
	mWanderDeltas.emplace_back(value);
	mHasChanged = true;
}

void Core::SteeringRMC::Seek(float value)
{
	mSeekDeltas.emplace_back(value);
	mHasChanged = true;
}

float Core::SteeringRMC::GetOrientationFromVelocity(const Core::Velocity& velocity)
{
	if ((velocity.mLinear.x == 0) && ((velocity.mLinear.z == 0)))
		return 0.f;
	return std::atan2(-velocity.mLinear.x, -velocity.mLinear.z);
}

void Core::SteeringRMC::Steering(TransformMC* pTransform, Core::Velocity& velocity, Core::Velocity steeringVelocity, float delta)
{
	using namespace DirectX;

	XMVECTOR vel{ XMLoadFloat3(&velocity.mLinear) };
	XMVECTOR force{ XMLoadFloat3(&steeringVelocity.mLinear) - vel };
	XMVECTOR acc{ force /* TODO: divive by mass */ };
	vel += acc * delta;
	XMStoreFloat3(&velocity.mLinear, vel);
	//XMVECTOR pos{ XMLoadFloat4(&pTransform->GetWorldPosition()) + vel };
	//XMFLOAT4 newPos{};
	//XMStoreFloat4(&newPos, pos);
	//pTransform->SetPosition(newPos);
	pTransform->Move(delta, velocity.mLinear);

	XMVECTOR rot{ XMQuaternionRotationAxis(XMVECTOR{ 0.f, 1.f, 0.f }, GetOrientationFromVelocity(velocity)) };
	XMFLOAT4 newRot{};
	XMStoreFloat4(&newRot, rot);
	pTransform->SetRotation(newRot);
}

Core::Velocity Core::SteeringRMC::CalculateWander(Core::WanderData& wanderData, Core::Velocity& velocity)
{
	using namespace DirectX;

	XMVECTOR offset{ wanderData.mOffset * XMVector3Normalize(XMLoadFloat3(&velocity.mLinear)) };
	XMVECTOR circleOffset{ std::cos(wanderData.mAngle) * wanderData.mRadius, 0.f, std::sin(wanderData.mAngle) * wanderData.mRadius };
	wanderData.mAngle += wanderData.mAngleChange * ((float(std::rand()) / float(RAND_MAX)) - .5f);
	XMVECTOR vel{ XMVector3Normalize(offset + circleOffset) * mMaxSpeed };
	Core::Velocity retVelocity{};
	XMStoreFloat3(&retVelocity.mLinear, vel);
	return retVelocity;
}

Core::Velocity Core::SteeringRMC::CalculateSeek(TransformMC* pTransform, TransformMC* pTarget)
{
	using namespace DirectX;

	XMVECTOR vel{ XMVector4Normalize(XMLoadFloat4(&pTarget->GetWorldPosition()) - XMLoadFloat4(&pTransform->GetWorldPosition())) * mMaxSpeed };
	Core::Velocity retVelocity{};
	XMStoreFloat3(&retVelocity.mLinear, vel);
	return retVelocity;
}
