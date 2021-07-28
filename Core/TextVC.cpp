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

void Core::TextVC::Render(Core::Canvas* pCanvas, Core::Material2D* pMaterial)
{
#ifdef _DEBUG
	if (!mIsInitialized)
	{
		mpObject->GetScene()->GetApplication()->GetServiceLocator().GetLogger()->Message(this, Core::MsgType::Fatal, "TriangleVC.Render() while mIsInitialized == false");
		return;
	}
#endif
	(pMaterial);
	if (!mIsActive)
		return;
	Core::Application* pApp{ mpObject->GetScene()->GetApplication() };
	auto pD2d1DeviceContext{ pApp->GetD2d1DeviceContext() };

	pD2d1DeviceContext->DrawText(
		mText.c_str(),
		(UINT32)mText.length(),
		pApp->GetDWriteTextFormat().Get(),
		mRect,
		pCanvas->GetBrush().Get());
}

void Core::TextVC::SetText(const std::wstring& text)
{
	mText = text;
}

void Core::TextVC::SetRect(const D2D1_RECT_F& rect)
{
	mRect = rect;
}
