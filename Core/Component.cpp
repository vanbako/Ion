#include "pch.h"
#include "Component.h"

using namespace Ion;

Core::Component::Component(bool isActive, Core::Object* pObject)
	: mIsInitialized{ false }
	, mIsActive{ isActive }
	, mpObject{ pObject }
{
}

void Core::Component::Initialize()
{
	mIsInitialized = true;
}

void Core::Component::SetIsActive(bool isActive)
{
	mIsActive = isActive;
}

const bool Core::Component::GetIsActive() const
{
	return mIsActive;
}

void Core::Component::SetObject(Core::Object* pObject)
{
	mpObject = pObject;
}

Core::Object* Core::Component::GetObject()
{
	return mpObject;
}
