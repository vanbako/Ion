#include "pch.h"
#include "TriangleVC.h"
#include "Application.h"
#include "Material2D.h"
#include "Object.h"
#include "Scene.h"

using namespace Ion::Core;

TriangleVC::TriangleVC(bool isActive, Object* pObject)
	: ViewC(isActive, pObject, "", "Brush")
{
}

void TriangleVC::Initialize()
{
	if (mIsInitialized)
		return;
	Application* pApplication{ mpObject->GetScene()->GetApplication() };
	auto pD2d1Factory{ pApplication->GetD2d1Factory() };

	Microsoft::WRL::ComPtr<ID2D1GeometrySink> pSink{};
	ThrowIfFailed(pD2d1Factory->CreatePathGeometry(&mpPathGeometry));
	ThrowIfFailed(mpPathGeometry->Open(&pSink));
	pSink->BeginFigure(D2D1_POINT_2F{ 10.f, 0.f }, D2D1_FIGURE_BEGIN_HOLLOW);
	pSink->AddLine(D2D1_POINT_2F{ 0.f, 16.f });
	pSink->AddLine(D2D1_POINT_2F{ 20.f, 16.f });
	pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
	ThrowIfFailed(pSink->Close());
	mIsInitialized = true;
}

void TriangleVC::Update(float delta)
{
	(delta);
	if (!mIsActive)
		return;
}

void TriangleVC::Render(Canvas* pCanvas, Material2D* pMaterial)
{
	(pMaterial);
	if (!mIsActive)
		return;
	Application* pApp{ mpObject->GetScene()->GetApplication() };
	auto pD2d1DeviceContext{ pApp->GetD2d1DeviceContext() };

	pD2d1DeviceContext->DrawGeometry(mpPathGeometry.Get(), pCanvas->GetBrush().Get(), 2.f);
}
