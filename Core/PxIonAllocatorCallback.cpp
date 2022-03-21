#include "pch.h"
#include "PxIonAllocatorCallback.h"

void* physx::PxIonAllocatorCallback::allocate(std::size_t size, const char* typeName, const char* filename, int line)
{
    (typeName);
    (filename);
    (line);
    return _aligned_malloc(size, 16);
}

void physx::PxIonAllocatorCallback::deallocate(void* ptr)
{
    _aligned_free(ptr);
}
