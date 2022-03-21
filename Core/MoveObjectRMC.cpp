#include "pch.h"
#include "MoveObjectRMC.h"
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

const std::string Core::MoveObjectRMC::mName{ "MoveObject" };

Core::MoveObjectRMC::MoveObjectRMC(bool isActive, Core::Object* pObject)
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
	, mpTransform{ nullptr }
{
}

Core::MoveObjectRMC::~MoveObjectRMC()
{
	for (std::pair<std::string, Core::Command*>& command : mCommands)
		delete command.second;
}

void Core::MoveObjectRMC::Initialize()
{
	Core::ModelC::Initialize();
}

void Core::MoveObjectRMC::Update(float delta)
{
	using namespace DirectX;
	if (!mIsActive)
		return;
	Core::ModelC::Update(delta);
	if (mpTransform == nullptr)
	{
		mpTransform = mpObject->GetModelC<Core::TransformMC>();
		mHasChanged = true;
	}
	if (mpTransform == nullptr)
		return;
	if (!mHasChanged)
		return;

	DirectX::XMFLOAT4 pos{ mpTransform->GetWorldPosition() };
	float walkSpeed{ 50.f };
	float rotateSpeed{ 1.f };
	float moveDelta{ walkSpeed * delta };
	float rotateDelta{ rotateSpeed * delta };
	if (mMoveForward[mCurrent])
	{
		pos.z += moveDelta;
		mpTransform->SetPosition(pos);
	}
	if (mMoveBack[mCurrent])
	{
		pos.z -= moveDelta;
		mpTransform->SetPosition(pos);
	}
	if (mMoveLeft[mCurrent])
	{
		pos.x -= moveDelta;
		mpTransform->SetPosition(pos);
	}
	if (mMoveRight[mCurrent])
	{
		pos.x += moveDelta;
		mpTransform->SetPosition(pos);
	}
	if (mMoveUp[mCurrent])
	{
		pos.y += moveDelta;
		mpTransform->SetPosition(pos);
	}
	if (mMoveDown[mCurrent])
	{
		pos.y -= moveDelta;
		mpTransform->SetPosition(pos);
	}
	if (mRotateLeft[mCurrent])
	{
		DirectX::XMFLOAT4 rot{ mpTransform->GetRotation() };
		DirectX::XMVECTOR axis{ 0.f, 1.f, 0.f };
		DirectX::XMVECTOR rotVec{ XMQuaternionMultiply(
			XMLoadFloat4(&rot),
			DirectX::XMQuaternionRotationAxis(axis, rotateDelta)
		) };
		XMStoreFloat4(&rot, rotVec);
		mpTransform->SetRotation(rot);
	}
	if (mRotateRight[mCurrent])
	{
		DirectX::XMFLOAT4 rot{ mpTransform->GetRotation() };
		DirectX::XMVECTOR axis{ 0.f, 1.f, 0.f };
		DirectX::XMVECTOR rotVec{ XMQuaternionMultiply(
			XMLoadFloat4(&rot),
			DirectX::XMQuaternionRotationAxis(axis, -rotateDelta)
		) };
		XMStoreFloat4(&rot, rotVec);
		mpTransform->SetRotation(rot);
	}
}

const std::vector<std::pair<std::string, Core::Command*>>& Core::MoveObjectRMC::GetCommands() const
{
	return mCommands;
}

const std::string& Core::MoveObjectRMC::GetName() const
{
	return mName;
}

void Core::MoveObjectRMC::SetTransformMC(TransformMC* pTransform)
{
	mpTransform = pTransform;
}
