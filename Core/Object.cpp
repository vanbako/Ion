#include "pch.h"
#include "Object.h"
#include "ControllerC.h"
#include "ViewC.h"

using namespace Ion::Core;

Object::Object(bool isActive, Scene* pScene, Object* pParent)
	: mIsActive{ isActive }
	, mpScene{ pScene }
	, mpParentObject{ pParent }
	, mChildObjects{}
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

void Object::SetIsActive(bool isActive, bool includeComponents)
{
	mIsActive = isActive;
	for (auto& childObject : mChildObjects)
		childObject.SetIsActive(isActive);
	if (includeComponents)
	{
		for (auto& pModelC : mpModelCs)
			pModelC->SetIsActive(isActive);
		for (auto& pControllerC : mpControllerCs)
			pControllerC->SetIsActive(isActive);
		for (auto& pViewC : mpViewCs)
			pViewC->SetIsActive(isActive);
	}
}

const bool Object::GetIsActive() const
{
	return mIsActive;
}

Scene* Object::GetScene()
{
	return mpScene;
}

Object* Object::GetParent()
{
	return mpParentObject;
}

Object* Object::AddChildObject(bool isActive)
{
	mChildObjects.emplace_back(isActive, mpScene, this);
	Core::Object* pObject{ &mChildObjects.back() };
	return pObject;
}

void Object::ModelCInitialize()
{
	for (ModelC* pModelC : mpModelCs)
		pModelC->Initialize();
	for (auto& childObject : mChildObjects)
		childObject.ModelCInitialize();
}

void Object::ModelCUpdate(float delta)
{
	for (ModelC* pModelC : mpModelCs)
		pModelC->Update(delta);
	for (auto& childObject : mChildObjects)
		childObject.ModelCUpdate(delta);
}

void Object::ModelCSwitch()
{
	for (ModelC* pModelC : mpModelCs)
		pModelC->Switch();
	for (auto& childObject : mChildObjects)
		childObject.ModelCSwitch();
}

void Object::ControllerCInitialize()
{
	for (ControllerC* pControllerC : mpControllerCs)
		pControllerC->Initialize();
	for (auto& childObject : mChildObjects)
		childObject.ControllerCInitialize();
}

void Object::ControllerCUpdate(float delta)
{
	for (ControllerC* pControllerC : mpControllerCs)
		pControllerC->Update(delta);
	for (auto& childObject : mChildObjects)
		childObject.ControllerCUpdate(delta);
}

void Object::ViewCInitialize()
{
	for (ViewC* pViewC : mpViewCs)
		pViewC->Initialize();
	for (auto& childObject : mChildObjects)
		childObject.ViewCInitialize();
}

void Object::ViewCUpdate(float delta)
{
	for (ViewC* pViewC : mpViewCs)
		pViewC->Update(delta);
	for (auto& childObject : mChildObjects)
		childObject.ViewCUpdate(delta);
}
