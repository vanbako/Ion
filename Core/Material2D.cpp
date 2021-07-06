#include "../Core/pch.h"
#include "../Core/Material2D.h"
#include "../Core/Application.h"
#include "../Core/Canvas.h"

using namespace Ion::Core;

Material2D::Material2D(Application* pApplication, const std::string& name)
	: mIsInitialized{ false }
	, mpApplication{ pApplication }
	, mName{ name }
	, mpCanvasViewCs{}
{
}

void Material2D::Initialize()
{
	if (mIsInitialized)
		return;
	auto pD2d1Factory{ mpApplication->GetD2d1Factory() };
	mIsInitialized = true;
}

void Material2D::Render(Canvas* pCanvas)
{
	for (auto pViewC : mpCanvasViewCs[pCanvas])
		pViewC->Render(pCanvas, this);
}

void Material2D::AddViewC(Canvas* pCanvas, ViewC* pViewC)
{
	mpCanvasViewCs[pCanvas].emplace_back(pViewC);
}
