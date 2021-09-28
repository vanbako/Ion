#include "pch.h"
#include "TriangleVC.h"
#include "Application.h"
#include "Material2D.h"
#include "Object.h"
#include "Scene.h"

using namespace Ion;

Core::TriangleVC::TriangleVC(bool isActive, Core::Object* pObject)
	: Core::ViewC(isActive, pObject, "", "Brush")
{
}

void Core::TriangleVC::Initialize()
{
	if (mIsInitialized)
		return;
	//Core::Application* pApplication{ mpObject->GetScene()->GetApplication() };
	//auto pD2d1Factory{ pApplication->GetD2d1Factory() };

	//Microsoft::WRL::ComPtr<ID2D1GeometrySink> pSink{};
	//mpObject->GetScene()->GetApplication()->ThrowIfFailed(pD2d1Factory->CreatePathGeometry(&mpPathGeometry));
	//mpObject->GetScene()->GetApplication()->ThrowIfFailed(mpPathGeometry->Open(&pSink));
	//pSink->BeginFigure(D2D1_POINT_2F{ 10.f, 0.f }, D2D1_FIGURE_BEGIN_HOLLOW);
	//pSink->AddLine(D2D1_POINT_2F{ 0.f, 16.f });
	//pSink->AddLine(D2D1_POINT_2F{ 20.f, 16.f });
	//pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
	//mpObject->GetScene()->GetApplication()->ThrowIfFailed(pSink->Close());
	mIsInitialized = true;
}

void Core::TriangleVC::Update(float delta)
{
	(delta);
	if (!mIsActive)
		return;
}

bool Core::TriangleVC::Render(Core::Canvas* pCanvas, Core::Material2D* pMaterial)
{
	if (!mIsInitialized)
	{
#ifdef ION_LOGGER
		mpObject->GetScene()->GetApplication()->GetServiceLocator().GetLogger()->Message(typeid(this).name(), Core::MsgType::Fatal, "TriangleVC.Render() while mIsInitialized == false");
#endif
		return false;
	}
	if (!Core::ViewC::Render(pCanvas, pMaterial))
		return false;
	if (!mIsActive)
		return false;
	//Core::Application* pApp{ mpObject->GetScene()->GetApplication() };
	//auto pD2d1DeviceContext{ pApp->GetD2d1DeviceContext() };

	//pD2d1DeviceContext->DrawGeometry(mpPathGeometry.Get(), pCanvas->GetBrush().Get(), 2.f);
	return true;
}
