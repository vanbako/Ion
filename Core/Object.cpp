#include "pch.h"
#include "Object.h"
#include "ControllerC.h"
#include "ViewC.h"

using namespace Ion;

Core::Object::Object(bool isActive, Core::Scene* pScene, Core::Object* pParent)
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

Core::Object::~Object()
{
	for (Core::ModelC* pModelC : mpModelCs)
		delete pModelC;
	for (Core::ControllerC* pControllerC : mpControllerCs)
		delete pControllerC;
	for (Core::ViewC* pViewC : mpViewCs)
		delete pViewC;
}

void Core::Object::SetIsActive(bool isActive, bool includeComponents)
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

const bool Core::Object::GetIsActive() const
{
	return mIsActive;
}

Core::Scene* Core::Object::GetScene()
{
	return mpScene;
}

Core::Object* Core::Object::GetParent()
{
	return mpParentObject;
}

Core::Object* Core::Object::AddChildObject(bool isActive)
{
	return &mChildObjects.emplace_back(isActive, mpScene, this);
}

void Core::Object::ModelCInitialize()
{
	for (Core::ModelC* pModelC : mpModelCs)
		pModelC->Initialize();
	for (auto& childObject : mChildObjects)
		childObject.ModelCInitialize();
}

void Core::Object::ModelCUpdate(float delta)
{
	for (Core::ModelC* pModelC : mpModelCs)
		pModelC->Update(delta);
	for (auto& childObject : mChildObjects)
		childObject.ModelCUpdate(delta);
}

void Core::Object::ModelCSwitch()
{
	for (Core::ModelC* pModelC : mpModelCs)
		pModelC->Switch();
	for (auto& childObject : mChildObjects)
		childObject.ModelCSwitch();
}

void Core::Object::ControllerCInitialize()
{
	for (Core::ControllerC* pControllerC : mpControllerCs)
		pControllerC->Initialize();
	for (auto& childObject : mChildObjects)
		childObject.ControllerCInitialize();
}

void Core::Object::ControllerCUpdate(float delta)
{
	for (Core::ControllerC* pControllerC : mpControllerCs)
		pControllerC->Update(delta);
	for (auto& childObject : mChildObjects)
		childObject.ControllerCUpdate(delta);
}

void Core::Object::ViewCInitialize()
{
	for (Core::ViewC* pViewC : mpViewCs)
		pViewC->Initialize();
	for (auto& childObject : mChildObjects)
		childObject.ViewCInitialize();
}

void Core::Object::ViewCUpdate(float delta)
{
	for (Core::ViewC* pViewC : mpViewCs)
		pViewC->Update(delta);
	for (auto& childObject : mChildObjects)
		childObject.ViewCUpdate(delta);
}
