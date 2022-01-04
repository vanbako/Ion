#include "pch.h"
#include "ViewC.h"
#include "Object.h"
#include "Scene.h"
#include "Application.h"
#include "Material3D.h"
#include "Material2D.h"
#include "Canvas.h"

using namespace Ion;

Core::ViewC::ViewC(bool isActive, Core::Object* pObject, const std::string& material3D, const std::string& material2D)
	: Core::Component(isActive, pObject)
	, mMaterial3D{ material3D }
	, mMaterial2D{ material2D }
	, mpMaterial3D{ nullptr }
	, mpMaterial2D{ nullptr }
	, mpCanvases{}
{
	if (material3D != "")
	{
		mpMaterial3D = pObject->GetScene()->GetApplication()->GetResource<Material3D>()->AddResource(material3D);
		mpMaterial3D->Initialize();
	}
	if (material2D != "")
	{
		mpMaterial2D = pObject->GetScene()->GetApplication()->GetResource<Material2D>()->AddResource(material2D);
		mpMaterial2D->Initialize();
	}
}

Core::ViewC::~ViewC()
{
	if (mMaterial3D != "")
		mpObject->GetScene()->GetApplication()->GetResource<Material3D>()->RemoveResource(mMaterial3D);
	if (mMaterial2D != "")
		mpObject->GetScene()->GetApplication()->GetResource<Material2D>()->RemoveResource(mMaterial2D);
}

void Core::ViewC::AddCanvas(Core::Canvas* pCanvas)
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

bool Core::ViewC::Render(Core::Canvas* pCanvas, Core::Material3D* pMaterial)
{
	(pCanvas);
	(pMaterial);
	return mpObject->GetScene()->GetIsActive();
}

bool Core::ViewC::Render(Core::Canvas* pCanvas, Core::Material2D* pMaterial)
{
	(pCanvas);
	(pMaterial);
	return mpObject->GetScene()->GetIsActive();
}
