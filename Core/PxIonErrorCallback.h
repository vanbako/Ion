#pragma once

#include "foundation/PxErrorCallback.h"
#include "foundation/PxErrors.h"

namespace physx
{
	class PxIonErrorCallback
		: public PxErrorCallback
	{
	public:
		virtual ~PxIonErrorCallback() = default;
		virtual void reportError(PxErrorCode::Enum code, const char* message, const char* file, int line) override;
	};

}
