#include "../Core/pch.h"
#include "../Core/CameraMC.h"
#include "../Core/TransformMC.h"
#include "../Core/Object.h"
#include "../Core/Scene.h"

using namespace Ion::Core;

CameraMC::CameraMC(bool isActive, Object* pObject)
	: ModelC(isActive, pObject)
	, mpCanvas{ nullptr }
	, mFarPlane{ 2500.f }
	, mNearPlane{ 0.1f }
	, mFOV{ DirectX::XM_PIDIV4 }
	, mViewProjection{}
{
}

void CameraMC::Initialize()
{
	mIsInitialized = true;
}

void CameraMC::Update(float delta)
{
	(delta);
	if (!mIsActive)
		return;
	if (mpCanvas == nullptr)
		return;
	using namespace DirectX;

	TransformMC* pTranformMC{ mpObject->GetModelC<TransformMC>() };
	DirectX::XMMATRIX projection{ DirectX::XMMatrixPerspectiveFovLH(mFOV, mpCanvas->GetRatio(), mNearPlane, mFarPlane) };
	const DirectX::XMVECTOR worldPosition{ DirectX::XMLoadFloat3(&pTranformMC->GetWorldPosition()) };
	const DirectX::XMVECTOR forward{ DirectX::XMLoadFloat3(&pTranformMC->GetForward()) };
	const DirectX::XMVECTOR up{ DirectX::XMLoadFloat3(&pTranformMC->GetUp()) };

	const DirectX::XMMATRIX view{ DirectX::XMMatrixLookAtLH(worldPosition, worldPosition + forward, up) };

	DirectX::XMStoreFloat4x4(&mViewProjection, view * projection);
}

void CameraMC::Switch()
{
	if (!mIsActive)
		return;
}

void CameraMC::SetCanvas(Canvas* pCanvas)
{
	mpCanvas = pCanvas;
}

const DirectX::XMFLOAT4X4& CameraMC::GetViewProjection()
{
	return mViewProjection;
}
