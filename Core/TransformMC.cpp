#include "../Core/pch.h"
#include "../Core/TransformMC.h"

using namespace Ion::Core;

TransformMC::TransformMC(bool isActive, Object* pObject)
	: ModelC(isActive, pObject)
	, mHasChanged{ true }
	, mCurrent{ 0 }
	, mPosition{ { 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f } }
	, mWorldPosition{}
	, mForward{}
	, mUp{}
	, mScale{ { 1.f, 1.f, 1.f }, { 1.f, 1.f, 1.f } }
	, mRotation{ { 0.f, 0.f, 0.f, 1.f }, { 0.f, 0.f, 0.f, 1.f } }
	, mWorld{}
{
}

void TransformMC::Initialize()
{
	if (mIsInitialized)
		return;
	InternalUpdate(0.f);
	InternalSwitch();
	mHasChanged = true;
	InternalUpdate(0.f);
	mIsInitialized = true;
}

void TransformMC::Update(float delta)
{
	(delta);
	if (!mIsActive)
		return;
	if (!mHasChanged)
		return;
	InternalUpdate(delta);
	mHasChanged = false;
}

void TransformMC::Switch()
{
	if (!mIsActive)
		return;
	InternalSwitch();
}

void TransformMC::SetPosition(const DirectX::XMFLOAT3& position)
{
	mPosition[mCurrent] = position;
	mHasChanged = true;
}

void TransformMC::SetScale(const DirectX::XMFLOAT3& scale)
{
	mScale[mCurrent] = scale;
	mHasChanged = true;
}

void TransformMC::SetRotation(const DirectX::XMFLOAT4& rotation)
{
	mRotation[mCurrent] = rotation;
	mHasChanged = true;
}

void TransformMC::SetRotation(const DirectX::XMFLOAT3& rotation, AngleUnit unit)
{
	switch (unit)
	{
	case AngleUnit::Degree:
		XMStoreFloat4(&mRotation[mCurrent], DirectX::XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z));
		break;
	}
}

const DirectX::XMFLOAT3& TransformMC::GetWorldPosition() const
{
	return mWorldPosition[mCurrent];
}

const DirectX::XMFLOAT3& TransformMC::GetForward() const
{
	return mForward[mCurrent];
}

const DirectX::XMFLOAT3& TransformMC::GetUp() const
{
	return mUp[mCurrent];
}

const DirectX::XMFLOAT4X4& TransformMC::GetWorld() const
{
	return mWorld[mCurrent];
}

void Ion::Core::TransformMC::InternalUpdate(float delta)
{
	(delta);
	DirectX::XMVECTOR rotation{ DirectX::XMLoadFloat4(&mRotation[mCurrent]) };
	DirectX::XMMATRIX world{
		DirectX::XMMatrixScaling(mScale[mCurrent].x, mScale[mCurrent].y, mScale[mCurrent].z) *
		DirectX::XMMatrixRotationQuaternion(rotation) *
		DirectX::XMMatrixTranslation(mPosition[mCurrent].x, mPosition[mCurrent].y, mPosition[mCurrent].z)
	};
	XMStoreFloat4x4(&mWorld[mCurrent], world);

	DirectX::XMVECTOR
		position{},
		scale{};
	if (DirectX::XMMatrixDecompose(&scale, &rotation, &position, world))
		DirectX::XMStoreFloat3(&mWorldPosition[mCurrent], position);
	const DirectX::XMMATRIX rotationMatrix{ DirectX::XMMatrixRotationQuaternion(rotation) };
	const DirectX::XMVECTOR forward{ DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f), rotationMatrix) };
	const DirectX::XMVECTOR right{ DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(1.f, 0.f, 0.f, 0.f), rotationMatrix) };
	DirectX::XMStoreFloat3(&mForward[mCurrent], forward);
	DirectX::XMStoreFloat3(&mUp[mCurrent], DirectX::XMVector3Cross(forward, right));
}

void TransformMC::InternalSwitch()
{
	int next{ 0 };
	if (mCurrent == 0)
		next = 1;
	mPosition[next] = mPosition[mCurrent];
	mWorldPosition[next] = mWorldPosition[mCurrent];
	mForward[next] = mForward[mCurrent];
	mUp[next] = mUp[mCurrent];
	mRotation[next] = mRotation[mCurrent];
	mScale[next] = mScale[mCurrent];
	mWorld[next] = mWorld[mCurrent];
	mCurrent = next;
}
