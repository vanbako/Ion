#include "pch.h"
#include "CameraRMC.h"
#include "TransformMC.h"
#include "Object.h"
#include "Scene.h"
#include "MoveForwardCmd.h"
#include "MoveBackCmd.h"
#include "MoveLeftCmd.h"
#include "MoveRightCmd.h"
#include "MoveUpCmd.h"
#include "MoveDownCmd.h"
#include "RotateLeftCmd.h"
#include "RotateRightCmd.h"
#include "KeyboardState.h"

using namespace Ion;

const std::string Core::CameraRMC::mName{ "Camera" };

Core::CameraRMC::CameraRMC(bool isActive, Core::Object* pObject)
	: Core::ReceiverMC(isActive, pObject)
	, mCommands{
		{ "MoveForward", new Core::MoveForwardCmd{ this } },
		{ "MoveBack", new Core::MoveBackCmd{ this } },
		{ "MoveLeft", new Core::MoveLeftCmd{ this } },
		{ "MoveRight", new Core::MoveRightCmd{ this } },
		{ "MoveUp", new Core::MoveUpCmd{ this } },
		{ "MoveDown", new Core::MoveDownCmd{ this } },
		{ "RotateLeft", new Core::RotateLeftCmd{ this } },
		{ "RotateRight", new Core::RotateRightCmd{ this } } }
	, mpCanvas{ nullptr }
	, mFarPlane{ 2500.f }
	, mNearPlane{ 0.1f }
	, mFOV{ DirectX::XM_PIDIV4 }
	, mView{}
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

Core::CameraRMC::~CameraRMC()
{
	for (std::pair<std::string, Core::Command*>& command : mCommands)
		delete command.second;
}

void Core::CameraRMC::Initialize()
{
	mIsInitialized = true;
}

void Core::CameraRMC::Update(float delta)
{
	(delta);
	using namespace DirectX;
	if (!mIsActive)
		return;
	if (mpCanvas == nullptr)
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

		DirectX::XMStoreFloat4x4(&mView, view);
		DirectX::XMStoreFloat4x4(&mViewProjection, view * projection);
	}
}

void Core::CameraRMC::Switch()
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
	if (mMoveForward[mCurrent] || mMoveBack[mCurrent] || mMoveLeft[mCurrent] || mMoveRight[mCurrent] || mMoveUp[mCurrent] || mMoveDown[mCurrent] || mRotateLeft[mCurrent] || mRotateRight[mCurrent])
		mHasChanged = true;
}

const std::vector<std::pair<std::string, Core::Command*>>& Core::CameraRMC::GetCommands() const
{
	return mCommands;
}

const std::string& Core::CameraRMC::GetName() const
{
	return mName;
}

void Core::CameraRMC::SetCanvas(Core::Canvas* pCanvas)
{
	mpCanvas = pCanvas;
}

const DirectX::XMFLOAT4X4& Core::CameraRMC::GetView() const
{
	return mView;
}

const DirectX::XMFLOAT4X4& Core::CameraRMC::GetViewProjection() const
{
	return mViewProjection;
}

void Core::CameraRMC::MoveForward(long long value)
{
	if (KeyboardState(value) == KeyboardState::KeyDown)
	{
		mMoveForward[mCurrent] = true;
		mHasChanged = true;
	}
	else if (KeyboardState(value) == KeyboardState::KeyUp)
		mMoveForward[mCurrent] = false;
}

void Core::CameraRMC::MoveBack(long long value)
{
	if (KeyboardState(value) == KeyboardState::KeyDown)
	{
		mMoveBack[mCurrent] = true;
		mHasChanged = true;
	}
	else if (KeyboardState(value) == KeyboardState::KeyUp)
		mMoveBack[mCurrent] = false;
}

void Core::CameraRMC::MoveLeft(long long value)
{
	if (KeyboardState(value) == KeyboardState::KeyDown)
	{
		mMoveLeft[mCurrent] = true;
		mHasChanged = true;
	}
	else if (KeyboardState(value) == KeyboardState::KeyUp)
		mMoveLeft[mCurrent] = false;
}

void Core::CameraRMC::MoveRight(long long value)
{
	if (KeyboardState(value) == KeyboardState::KeyDown)
	{
		mMoveRight[mCurrent] = true;
		mHasChanged = true;
	}
	else if (KeyboardState(value) == KeyboardState::KeyUp)
		mMoveRight[mCurrent] = false;
}

void Core::CameraRMC::MoveUp(long long value)
{
	if (KeyboardState(value) == KeyboardState::KeyDown)
	{
		mMoveUp[mCurrent] = true;
		mHasChanged = true;
	}
	else if (KeyboardState(value) == KeyboardState::KeyUp)
		mMoveUp[mCurrent] = false;
}

void Core::CameraRMC::MoveDown(long long value)
{
	if (KeyboardState(value) == KeyboardState::KeyDown)
	{
		mMoveDown[mCurrent] = true;
		mHasChanged = true;
	}
	else if (KeyboardState(value) == KeyboardState::KeyUp)
		mMoveDown[mCurrent] = false;
}

void Core::CameraRMC::RotateLeft(long long value)
{
	if (KeyboardState(value) == KeyboardState::KeyDown)
	{
		mRotateLeft[mCurrent] = true;
		mHasChanged = true;
	}
	else if (KeyboardState(value) == KeyboardState::KeyUp)
		mRotateLeft[mCurrent] = false;
}

void Core::CameraRMC::RotateRight(long long value)
{
	if (KeyboardState(value) == KeyboardState::KeyDown)
	{
		mRotateRight[mCurrent] = true;
		mHasChanged = true;
	}
	else if (KeyboardState(value) == KeyboardState::KeyUp)
		mRotateRight[mCurrent] = false;
}
