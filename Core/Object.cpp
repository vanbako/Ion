#include "../Core/pch.h"
#include "../Core/Object.h"
#include "../Core/ControllerC.h"
#include "../Core/ViewC.h"

using namespace Ion::Core;

Object::Object(bool isActive, Scene* pScene)
	: mIsActive{ isActive }
	, mpScene{ pScene }
	, mpObject{ nullptr }
	, mpTransformMC{ nullptr }
	, mpModelCs{}
	, mpControllerCs{}
	, mpViewCs{}
{
}

Object::~Object()
{
	for (ModelC* pModelC : mpModelCs)
		delete pModelC;
	for (ControllerC* pControllerC : mpControllerCs)
		delete pControllerC;
	for (ViewC* pViewC : mpViewCs)
		delete pViewC;
}

void Object::SetIsActive(bool isActive)
{
	mIsActive = isActive;
}

const bool Object::GetIsActive() const
{
	return mIsActive;
}

Scene* Object::GetScene()
{
	return mpScene;
}

void Object::ModelCInitialize()
{
	for (ModelC* pModelC : mpModelCs)
		pModelC->Initialize();
}

void Object::ModelCUpdate(float delta)
{
	for (ModelC* pModelC : mpModelCs)
		pModelC->Update(delta);
}

void Object::ModelCSwitch()
{
	for (ModelC* pModelC : mpModelCs)
		pModelC->Switch();
}

void Object::ControllerCInitialize()
{
	for (ControllerC* pControllerC : mpControllerCs)
		pControllerC->Initialize();
}

void Object::ControllerCUpdate(float delta)
{
	for (ControllerC* pControllerC : mpControllerCs)
		pControllerC->Update(delta);
}

void Object::ViewCInitialize()
{
	for (ViewC* pViewC : mpViewCs)
		pViewC->Initialize();
}

void Object::ViewCUpdate(float delta)
{
	for (ViewC* pViewC : mpViewCs)
		pViewC->Update(delta);
}
