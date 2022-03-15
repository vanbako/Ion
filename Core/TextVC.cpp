#include "pch.h"
#include "TextVC.h"
#include "Application.h"
#include "Material2D.h"
#include "Object.h"
#include "Scene.h"

using namespace Ion;

Core::TextVC::TextVC(bool isActive, Core::Object* pObject)
	: Core::ViewC(isActive, pObject, "", "Brush")
	, mText{}
	, mRect{ 40.f, 40.f, 680.f, 400.f }
{
}

void Core::TextVC::Initialize()
{
	if (mIsInitialized)
		return;
	mIsInitialized = true;
}

void Core::TextVC::Update(float delta)
{
	(delta);
	if (!mIsActive)
		return;
}

bool Core::TextVC::Render(Core::Canvas* pCanvas, Core::Material3D* pMaterial, float distSq)
{
	(pCanvas);
	(pMaterial);
	(distSq);
	return false;
}

bool Core::TextVC::Render(Core::Canvas* pCanvas, Core::Material2D* pMaterial)
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

	//pD2d1DeviceContext->DrawText(
	//	mText.c_str(),
	//	(UINT32)mText.length(),
	//	pApp->GetDWriteTextFormat().Get(),
	//	mRect,
	//	pCanvas->GetBrush().Get());
	return true;
}

void Core::TextVC::SetText(const std::wstring& text)
{
	mText = text;
}

void Core::TextVC::SetRect(const D2D1_RECT_F& rect)
{
	mRect = rect;
}
