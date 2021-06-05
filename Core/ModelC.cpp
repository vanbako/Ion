#include "../Core/pch.h"
#include "../Core/ModelC.h"

using namespace Ion::Core;

ModelC::ModelC(bool isActive, Object* pObject)
	: Component(isActive, pObject)
	, mCurrent{ 0 }
	, mHasChanged{ true }
{
}
