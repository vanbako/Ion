#include "../Core/pch.h"
#include "../Core/CameraRMC.h"
#include "../Core/TransformMC.h"
#include "../Core/Object.h"
#include "../Core/Scene.h"
#include "../Core/MoveForwardCmd.h"
#include "../Core/MoveBackCmd.h"
#include "../Core/MoveLeftCmd.h"
#include "../Core/MoveRightCmd.h"
#include "../Core/MoveUpCmd.h"
#include "../Core/MoveDownCmd.h"
#include "../Core/RotateLeftCmd.h"
#include "../Core/RotateRightCmd.h"

using namespace Ion::Core;

const std::string CameraRMC::mName{ "Camera" };

CameraRMC::CameraRMC(bool isActive, Object* pObject)
	: ReceiverMC(isActive, pObject)
	, mCommands{
		{ "MoveForward", new MoveForwardCmd{ this } },
		{ "MoveBack", new MoveBackCmd{ this } },
		{ "MoveLeft", new MoveLeftCmd{ this } },
		{ "MoveRight", new MoveRightCmd{ this } },
		{ "MoveUp", new MoveUpCmd{ this } },
		{ "MoveDown", new MoveDownCmd{ this } },
		{ "RotateLeft", new RotateLeftCmd{ this } },
		{ "RotateRight", new RotateRightCmd{ this } } }
	, mpCanvas{ nullptr }
	, mFarPlane{ 2500.f }
	, mNearPlane{ 0.1f }
	, mFOV{ DirectX::XM_PIDIV4 }
	, mViewProjection{}
	, mpTransform{ nullptr }
	, mMoveForward{ false, false }
	, mMoveBack{ false, false }
	, mMoveLeft{ false, false }
	, mMoveRight{ false, false }
	, mMoveUp{ false, false }
	, mMoveDown{ false, false }
	, mRotateLeft{ false, false }
	, mRotateRight{ false, false }
{
}

CameraRMC::~CameraRMC()
{
	for (std::pair<std::string, Command*>& command : mCommands)
		delete command.second;
}

void CameraRMC::Initialize()
{
	mIsInitialized = true;
}

void CameraRMC::Update(float delta)
{
	(delta);
	using namespace DirectX;
	if (!mIsActive)
		return;
	if (mpCanvas == nullptr)
		return;
	if (mpTransform == nullptr)
	{
		mpTransform = mpObject->GetModelC<TransformMC>();
		mHasChanged = true;
	}
	if (mpTransform == nullptr)
		return;
	if (!mHasChanged)
		return;

	{
		DirectX::XMFLOAT4 pos{ mpTransform->GetWorldPosition() };
		DirectX::XMFLOAT4 forward{ mpTransform->GetForward() };
		DirectX::XMFLOAT4 up{ mpTransform->GetUp() };
		DirectX::XMFLOAT4 right{ mpTransform->GetRight() };
		float walkSpeed{ 50.f };
		float rotateSpeed{ 2.f };
		float moveDelta{ walkSpeed * delta };
		float rotateDelta{ rotateSpeed * delta };
		if (mMoveForward[mCurrent])
		{
			DirectX::XMVECTOR s{ DirectX::XMVectorReplicate(moveDelta) };
			DirectX::XMVECTOR l{ DirectX::XMLoadFloat4(&forward) };
			DirectX::XMVECTOR p{ DirectX::XMLoadFloat4(&pos) };
			DirectX::XMStoreFloat4(&pos, DirectX::XMVectorMultiplyAdd(s, l, p));
		}
		if (mMoveBack[mCurrent])
		{
			DirectX::XMVECTOR s{ DirectX::XMVectorReplicate(-moveDelta) };
			DirectX::XMVECTOR l{ DirectX::XMLoadFloat4(&forward) };
			DirectX::XMVECTOR p{ DirectX::XMLoadFloat4(&pos) };
			DirectX::XMStoreFloat4(&pos, DirectX::XMVectorMultiplyAdd(s, l, p));
		}
		if (mMoveLeft[mCurrent])
		{
			DirectX::XMVECTOR s{ DirectX::XMVectorReplicate(-moveDelta) };
			DirectX::XMVECTOR r{ DirectX::XMLoadFloat4(&right) };
			DirectX::XMVECTOR p{ DirectX::XMLoadFloat4(&pos) };
			DirectX::XMStoreFloat4(&pos, DirectX::XMVectorMultiplyAdd(s, r, p));
		}
		if (mMoveRight[mCurrent])
		{
			DirectX::XMVECTOR s{ DirectX::XMVectorReplicate(moveDelta) };
			DirectX::XMVECTOR r{ DirectX::XMLoadFloat4(&right) };
			DirectX::XMVECTOR p{ DirectX::XMLoadFloat4(&pos) };
			DirectX::XMStoreFloat4(&pos, DirectX::XMVectorMultiplyAdd(s, r, p));
		}
		// MoveUp & Down can be quite simple, as long as the up vector aligns with the y-axis
		if (mMoveUp[mCurrent])
			pos.y += moveDelta;
		if (mMoveDown[mCurrent])
			pos.y -= moveDelta;
		if (mRotateLeft[mCurrent])
		{
			DirectX::XMMATRIX R{ DirectX::XMMatrixRotationY(rotateDelta) };
			DirectX::XMStoreFloat4(&right, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat4(&right), R));
			DirectX::XMStoreFloat4(&up, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat4(&up), R));
			DirectX::XMStoreFloat4(&forward, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat4(&forward), R));
			mpTransform->SetForward(forward);
			mpTransform->SetUp(up);
			mpTransform->SetRight(right);
		}
		if (mRotateRight[mCurrent])
		{
			DirectX::XMMATRIX R{ DirectX::XMMatrixRotationY(-rotateDelta) };
			DirectX::XMStoreFloat4(&right, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat4(&right), R));
			DirectX::XMStoreFloat4(&up, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat4(&up), R));
			DirectX::XMStoreFloat4(&forward, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat4(&forward), R));
			mpTransform->SetForward(forward);
			mpTransform->SetUp(up);
			mpTransform->SetRight(right);
		}
		mpTransform->SetPosition(pos);
	}
	{
		DirectX::XMMATRIX projection{ DirectX::XMMatrixPerspectiveFovLH(mFOV, mpCanvas->GetRatio(), mNearPlane, mFarPlane) };
		const DirectX::XMVECTOR worldPosition{ DirectX::XMLoadFloat4(&mpTransform->GetWorldPosition()) };
		const DirectX::XMVECTOR forward{ DirectX::XMLoadFloat4(&mpTransform->GetForward()) };
		const DirectX::XMVECTOR up{ DirectX::XMLoadFloat4(&mpTransform->GetUp()) };

		const DirectX::XMMATRIX view{ DirectX::XMMatrixLookAtLH(worldPosition, worldPosition + forward, up) };

		DirectX::XMStoreFloat4x4(&mViewProjection, view * projection);
	}
}

void CameraRMC::Switch()
{
	if (!mIsActive)
		return;
	if (!mHasChanged)
		return;
	mHasChanged = false;
	int next{ 0 };
	if (mCurrent == 0)
		next = 1;
	mMoveForward[next] = mMoveForward[mCurrent];
	mMoveBack[next] = mMoveBack[mCurrent];
	mMoveLeft[next] = mMoveLeft[mCurrent];
	mMoveRight[next] = mMoveRight[mCurrent];
	mMoveUp[next] = mMoveUp[mCurrent];
	mMoveDown[next] = mMoveDown[mCurrent];
	mRotateLeft[next] = mRotateLeft[mCurrent];
	mRotateRight[next] = mRotateRight[mCurrent];
	mMoveForward[mCurrent] = false;
	mMoveBack[mCurrent] = false;
	mMoveLeft[mCurrent] = false;
	mMoveRight[mCurrent] = false;
	mMoveUp[mCurrent] = false;
	mMoveDown[mCurrent] = false;
	mRotateLeft[mCurrent] = false;
	mRotateRight[mCurrent] = false;
}

const std::vector<std::pair<std::string, Command*>>& Ion::Core::CameraRMC::GetCommands() const
{
	return mCommands;
}

const std::string& Ion::Core::CameraRMC::GetName() const
{
	return mName;
}

void CameraRMC::SetCanvas(Canvas* pCanvas)
{
	mpCanvas = pCanvas;
}

const DirectX::XMFLOAT4X4& CameraRMC::GetViewProjection() const
{
	return mViewProjection;
}

void CameraRMC::MoveForward()
{
	mMoveForward[mCurrent] = true;
	mHasChanged = true;
}

void CameraRMC::MoveBack()
{
	mMoveBack[mCurrent] = true;
	mHasChanged = true;
}

void CameraRMC::MoveLeft()
{
	mMoveLeft[mCurrent] = true;
	mHasChanged = true;
}

void CameraRMC::MoveRight()
{
	mMoveRight[mCurrent] = true;
	mHasChanged = true;
}

void CameraRMC::MoveUp()
{
	mMoveUp[mCurrent] = true;
	mHasChanged = true;
}

void CameraRMC::MoveDown()
{
	mMoveDown[mCurrent] = true;
	mHasChanged = true;
}

void CameraRMC::RotateLeft()
{
	mRotateLeft[mCurrent] = true;
	mHasChanged = true;
}

void CameraRMC::RotateRight()
{
	mRotateRight[mCurrent] = true;
	mHasChanged = true;
}
