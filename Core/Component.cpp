#include "../Core/pch.h"
#include "../Core/Component.h"

using namespace Ion::Core;

Component::Component(bool isActive, Object* pObject)
	: mIsInitialized{ false }
	, mIsActive{ isActive }
	, mpObject{ pObject }
{
}

void Component::Initialize()
{
	mIsInitialized = true;
}

void Component::SetIsActive(bool isActive)
{
	mIsActive = isActive;
}

const bool Component::GetIsActive() const
{
	return mIsActive;
}

void Component::SetObject(Object* pObject)
{
	mpObject = pObject;
}

Object* Component::GetObject()
{
	return mpObject;
}
