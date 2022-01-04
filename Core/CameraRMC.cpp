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

using namespace Ion;

const std::string Core::CameraRMC::mName{ "Camera" };

Core::CameraRMC::CameraRMC(bool isActive, Core::Object* pObject)
	: Core::MoveRMC(isActive, pObject)
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
		DirectX::XMFLOAT4 fw{ mpTransform->GetForward() };
		fw.w = 0.f;
		const DirectX::XMVECTOR forward{ DirectX::XMLoadFloat4(&fw) };
		const DirectX::XMVECTOR up{ DirectX::XMLoadFloat4(&mpTransform->GetUp()) };

		const DirectX::XMMATRIX view{ DirectX::XMMatrixLookAtLH(worldPosition, worldPosition + forward, up) };
		const DirectX::XMMATRIX viewInv{ DirectX::XMMatrixInverse(nullptr, view * projection) };

		DirectX::XMStoreFloat4x4(&mView, view);
		DirectX::XMStoreFloat4x4(&mViewProjection, view * projection);
		DirectX::XMStoreFloat4x4(&mViewInverse, viewInv);
	}
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

const DirectX::XMFLOAT4X4& Core::CameraRMC::GetViewInverse() const
{
	return mViewInverse;
}

Core::TransformMC* Core::CameraRMC::GetTransform()
{
	return mpTransform;
}
