#include "pch.h"
#include "Material2D.h"
#include "Application.h"
#include "Canvas.h"

using namespace Ion;

Core::Material2D::Material2D(Core::Application* pApplication, const std::string& name)
	: Core::Material(pApplication, name)
	, mpCanvasViewCs{}
{
}

void Core::Material2D::Initialize()
{
	if (mIsInitialized)
		return;
	Core::Material::Initialize();
	//auto pD2d1Factory{ mpApplication->GetD2d1Factory() };
}

void Core::Material2D::Render(Core::Canvas* pCanvas)
{
	if (!mIsInitialized)
	{
#ifdef ION_LOGGER
		mpApplication->GetServiceLocator().GetLogger()->Message(typeid(this).name(), Core::MsgType::Fatal, "Material2D.Render() while mIsInitialized == false");
#endif
		return;
	}
	for (auto pViewC : mpCanvasViewCs[pCanvas])
		pViewC->Render(pCanvas, this);
}

void Core::Material2D::AddViewC(Core::Canvas* pCanvas, Core::ViewC* pViewC)
{
	mpCanvasViewCs[pCanvas].emplace_back(pViewC);
}
