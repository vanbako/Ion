#include "pch.h"
#include "ControllerC.h"
#include "Object.h"
#include "Scene.h"

using namespace Ion;

Core::ControllerC::ControllerC(bool isActive, Core::Object* pObject)
	: Core::Component(isActive, pObject)
{
}

void Core::ControllerC::Initialize()
{
	Core::Component::Initialize();
	mpObject->GetScene()->AddControllerC(this);
}

void Core::ControllerC::Update(float delta)
{
	(delta);
	if (mpCube == nullptr)
		return;
	Core::TransformMC* pTransform{ mpObject->GetModelC<Core::TransformMC>() };
	if (pTransform == nullptr || !pTransform->GetHasMoved())
		return;
	//pTransform->SetHasMoved(false);
	mpObject->GetScene()->MoveControllerC(this, (Core::ControllerCCube*)mpCube);
}
