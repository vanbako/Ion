#include "pch.h"
#include "ViewC.h"
#include "Object.h"
#include "Scene.h"
#include "Application.h"
#include "Material3D.h"
#include "Material3DResource.h"
#include "Material2D.h"
#include "Material2DResource.h"
#include "Canvas.h"

using namespace Ion;

Core::ViewC::ViewC(bool isActive, Core::Object* pObject, const std::string& material3D, const std::string& material2D)
	: Core::Component(isActive, pObject)
	, mMaterial3D{ material3D }
	, mMaterial2D{ material2D }
	, mpMaterial3D{ nullptr }
	, mpMaterial2D{ nullptr }
	, mpCanvases{}
	//, mpCube{ nullptr }
{
	if (material3D != "")
	{
		mpMaterial3D = pObject->GetScene()->GetApplication()->GetResourceManager()->GetResource<Material3DResource>()->AddMaterial3D(material3D);
		mpMaterial3D->Initialize();
	}
	if (material2D != "")
	{
		mpMaterial2D = pObject->GetScene()->GetApplication()->GetResourceManager()->GetResource<Material2DResource>()->AddMaterial2D(material2D);
		mpMaterial2D->Initialize();
	}
}

Core::ViewC::~ViewC()
{
	if (mMaterial3D != "")
		mpObject->GetScene()->GetApplication()->GetResourceManager()->GetResource<Material3DResource>()->RemoveMaterial3D(mMaterial3D);
	if (mMaterial2D != "")
		mpObject->GetScene()->GetApplication()->GetResourceManager()->GetResource<Material2DResource>()->RemoveMaterial2D(mMaterial2D);
}

void Core::ViewC::Update(float)
{
	if (!mIsActive || (mpMaterial3D == nullptr) || (mpCube == nullptr))
		return;
	TransformMC* pTransform{ mpObject->GetModelC<TransformMC>() };
	if (!pTransform->GetHasMoved())
		return;
	//pTransform->SetHasMoved(false);
	for (Core::Canvas* pCanvas : mpCanvases)
		mpMaterial3D->MoveViewC(pCanvas, this, (Core::ViewCCube*)mpCube);
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

bool Core::ViewC::Render(Core::Canvas* pCanvas, Core::Material3D* pMaterial, float dist)
{
	(pCanvas);
	(pMaterial);
	(dist);
	return mpObject->GetScene()->GetIsActive();
}

bool Core::ViewC::Render(Core::Canvas* pCanvas, Core::Material2D* pMaterial)
{
	(pCanvas);
	(pMaterial);
	return mpObject->GetScene()->GetIsActive();
}
