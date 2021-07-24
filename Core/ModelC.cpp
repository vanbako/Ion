#include "pch.h"
#include "ModelC.h"

using namespace Ion;

Core::ModelC::ModelC(bool isActive, Core::Object* pObject)
	: Core::Component(isActive, pObject)
	, mCurrent{ 0 }
	, mHasChanged{ true }
{
}
