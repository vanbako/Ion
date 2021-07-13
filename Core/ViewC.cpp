#include "pch.h"
#include "ViewC.h"
#include "Object.h"
#include "Scene.h"
#include "Application.h"
#include "Material3D.h"
#include "Material2D.h"
#include "Canvas.h"

using namespace Ion::Core;

ViewC::ViewC(bool isActive, Object* pObject, const std::string& material3D, const std::string& material2D)
	: Component(isActive, pObject)
	, mpMaterial3D{ nullptr }
	, mpMaterial2D{ nullptr }
	, mpCanvases{}
{
	if (material3D != "")
	{
		mpMaterial3D = pObject->GetScene()->GetApplication()->AddMaterial3D(material3D);
		mpMaterial3D->Initialize();
	}
	if (material2D != "")
	{
		mpMaterial2D = pObject->GetScene()->GetApplication()->AddMaterial2D(material2D);
		mpMaterial2D->Initialize();
	}
}

void ViewC::AddCanvas(Canvas* pCanvas)
{
	if (mpMaterial3D != nullptr)
		mpMaterial3D->AddViewC(pCanvas, this);
 	if (mpMaterial2D != nullptr)
		mpMaterial2D->AddViewC(pCanvas, this);
	mpCanvases.emplace(pCanvas);
	if (mpMaterial3D != nullptr)
		pCanvas->AddMaterial(mpMaterial3D);
	if (mpMaterial2D != nullptr)
		pCanvas->AddMaterial(mpMaterial2D);
}
