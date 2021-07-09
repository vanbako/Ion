#include "../Core/pch.h"
#include "../Core/PxIonAllocatorCallback.h"

using namespace physx;

void* PxIonAllocatorCallback::allocate(size_t size, const char* typeName, const char* filename, int line)
{
    (typeName);
    (filename);
    (line);
    return _aligned_malloc(size, 16);
}

void PxIonAllocatorCallback::deallocate(void* ptr)
{
    _aligned_free(ptr);
}
