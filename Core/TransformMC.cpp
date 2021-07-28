#include "pch.h"
#include "TransformMC.h"
#include "Object.h"
#include "Scene.h"

using namespace Ion;

Core::TransformMC::TransformMC(bool isActive, Core::Object* pObject)
	: ModelC(isActive, pObject)
	, mPosition{ { 0.f, 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f, 0.f } }
	, mWorldPosition{}
	, mForward{}
	, mUp{}
	, mRight{}
	, mScale{ { 1.f, 1.f, 1.f, 0.f }, { 1.f, 1.f, 1.f, 0.f } }
	, mRotation{ { 0.f, 0.f, 0.f, 1.f }, { 0.f, 0.f, 0.f, 1.f } }
	, mWorld{}
	, mPxRigidActor{ nullptr }
{
}

void Core::TransformMC::Initialize()
{
	if (mIsInitialized)
		return;
	InternalUpdate(0.f);
	InternalSwitch();
	mHasChanged = true;
	InternalUpdate(0.f);
	mIsInitialized = true;
}

void Core::TransformMC::Update(float delta)
{
	(delta);
	if (!mIsActive)
		return;
	if ((!mHasChanged) && (mPxRigidActor == nullptr))
		return;
 	if (mPxRigidActor != nullptr)
	{
		mpObject->GetScene()->GetPxScene()->lockRead();
		const physx::PxTransform& pxTransform{ mPxRigidActor->getGlobalPose()};
		mPosition[mCurrent] = DirectX::XMFLOAT4{ pxTransform.p.x, pxTransform.p.y, pxTransform.p.z, 0.f };
		mRotation[mCurrent] = DirectX::XMFLOAT4{ pxTransform.q.x, pxTransform.q.y, pxTransform.q.z, pxTransform.q.w };
		mpObject->GetScene()->GetPxScene()->unlockRead();
	}
	InternalUpdate(delta);
	mHasChanged = false;
}

void Core::TransformMC::Switch()
{
	if (!mIsActive)
		return;
	InternalSwitch();
}

void Core::TransformMC::SetPosition(const DirectX::XMFLOAT4& position)
{
	mPosition[mCurrent] = position;
	mHasChanged = true;
}

void Core::TransformMC::SetScale(const DirectX::XMFLOAT4& scale)
{
	mScale[mCurrent] = scale;
	mHasChanged = true;
}

void Core::TransformMC::SetRotation(const DirectX::XMFLOAT4& rotation)
{
	mRotation[mCurrent] = rotation;
	mHasChanged = true;
}

void Core::TransformMC::SetRotation(const DirectX::XMFLOAT3& rotation, AngleUnit unit)
{
	switch (unit)
	{
	case AngleUnit::Radian:
		DirectX::XMStoreFloat4(&mRotation[mCurrent], DirectX::XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z));
		break;
	case AngleUnit::Degree:
		DirectX::XMStoreFloat4(&mRotation[mCurrent], DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(rotation.x), DirectX::XMConvertToRadians(rotation.y), DirectX::XMConvertToRadians(rotation.z)));
		break;
	}
	DirectX::XMVECTOR rot{ DirectX::XMLoadFloat4(&mRotation[mCurrent]) };
	const DirectX::XMMATRIX rotationMatrix{ DirectX::XMMatrixRotationQuaternion(rot) };
	const DirectX::XMVECTOR forward{ DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f), rotationMatrix) };
	const DirectX::XMVECTOR right{ DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(1.f, 0.f, 0.f, 0.f), rotationMatrix) };
	DirectX::XMStoreFloat4(&mForward[mCurrent], forward);
	DirectX::XMStoreFloat4(&mUp[mCurrent], DirectX::XMVector3Cross(forward, right));
	DirectX::XMStoreFloat4(&mRight[mCurrent], right);
}

void Core::TransformMC::SetForward(const DirectX::XMFLOAT4& forward)
{
	mForward[mCurrent] = forward;
}

void Core::TransformMC::SetUp(const DirectX::XMFLOAT4& up)
{
	mUp[mCurrent] = up;
}

void Core::TransformMC::SetRight(const DirectX::XMFLOAT4& right)
{
	mRight[mCurrent] = right;
}

void Core::TransformMC::SetPxRigidActor(physx::PxRigidActor* pPxRigidActor)
{
	mPxRigidActor = pPxRigidActor;
}

const DirectX::XMFLOAT4& Core::TransformMC::GetRotation()
{
	return mRotation[mCurrent];
}

const DirectX::XMFLOAT4& Core::TransformMC::GetWorldPosition() const
{
	return mWorldPosition[mCurrent];
}

const DirectX::XMFLOAT4& Core::TransformMC::GetForward() const
{
	return mForward[mCurrent];
}

const DirectX::XMFLOAT4& Core::TransformMC::GetUp() const
{
	return mUp[mCurrent];
}

const DirectX::XMFLOAT4& Core::TransformMC::GetRight() const
{
	return mRight[mCurrent];
}

const DirectX::XMFLOAT4X4& Core::TransformMC::GetWorld() const
{
	return mWorld[mCurrent];
}

void Core::TransformMC::InternalUpdate(float delta)
{
	(delta);
	DirectX::XMVECTOR rotation{ DirectX::XMLoadFloat4(&mRotation[mCurrent]) };
	DirectX::XMMATRIX local{
		DirectX::XMMatrixScaling(mScale[mCurrent].x, mScale[mCurrent].y, mScale[mCurrent].z) *
		DirectX::XMMatrixRotationQuaternion(rotation) *
		DirectX::XMMatrixTranslation(mPosition[mCurrent].x, mPosition[mCurrent].y, mPosition[mCurrent].z)
	};
	DirectX::XMMATRIX world{ local };
	Core::Object* pParentObject{ mpObject->GetParent() };
	if (pParentObject != nullptr)
	{
		Core::TransformMC* pParentTransform{ pParentObject->GetModelC<Core::TransformMC>() };
		if (pParentTransform != nullptr)
		{
			DirectX::XMMATRIX parent{ DirectX::XMLoadFloat4x4(&pParentTransform->GetWorld()) };
			world = parent + local;
		}
	}
	XMStoreFloat4x4(&mWorld[mCurrent], world);

	DirectX::XMVECTOR
		position{},
		scale{};
	if (DirectX::XMMatrixDecompose(&scale, &rotation, &position, world))
		DirectX::XMStoreFloat4(&mWorldPosition[mCurrent], position);
}

void Core::TransformMC::InternalSwitch()
{
	int next{ 0 };
	if (mCurrent == 0)
		next = 1;
	mPosition[next] = mPosition[mCurrent];
	mWorldPosition[next] = mWorldPosition[mCurrent];
	mForward[next] = mForward[mCurrent];
	mUp[next] = mUp[mCurrent];
	mRight[next] = mRight[mCurrent];
	mRotation[next] = mRotation[mCurrent];
	mScale[next] = mScale[mCurrent];
	mWorld[next] = mWorld[mCurrent];
	mCurrent = next;
}
