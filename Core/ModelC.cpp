#include "pch.h"
#include "ModelC.h"
#include "Object.h"
#include "Scene.h"

using namespace Ion;

//Core::ModelC::ModelC()
//	: Core::Component(false, nullptr)
//{
//}

Core::ModelC::ModelC(bool isActive, Core::Object* pObject)
	: Core::Component(isActive, pObject)
	, mCurrent{ 0 }
	, mHasChanged{ true }
{
	//if (pObject != nullptr)
	//	pObject->GetScene()->AddModelC(this);
}

void Core::ModelC::Initialize()
{
	Core::Component::Initialize();
	mpObject->GetScene()->AddModelC(this);
}

void Core::ModelC::Update(float delta)
{
	(delta);
	if (mpCube == nullptr)
		return;
	Core::TransformMC* pTransform{ mpObject->GetModelC<Core::TransformMC>() };
	if (pTransform == nullptr || !pTransform->GetHasMoved())
		return;
	//pTransform->SetHasMoved(false);
	mpObject->GetScene()->MoveModelC(this, (Core::ModelCCube*)mpCube);
}
