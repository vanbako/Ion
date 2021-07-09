#pragma once

#include "foundation/PxAllocatorCallback.h"

namespace physx
{
	class PxIonAllocatorCallback
		: public PxAllocatorCallback
	{
	public:
		virtual ~PxIonAllocatorCallback() = default;
		virtual void* allocate(size_t size, const char* typeName, const char* filename, int line) override;
		virtual void deallocate(void* ptr) override;
	};

}
