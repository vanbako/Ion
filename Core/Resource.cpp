#include "pch.h"
#include "Resource.h"

using namespace Ion;

Core::Resource::Resource(Core::ResourceManager* pResourceManager)
	: mpResourceManager{ pResourceManager }
	, mMutexDuration{ 100 }
	, mMutex{}
	, mDeleteOnZero{ false }
#ifdef _DEBUG
	, mReferenceCounts{}
#endif
{
}

void Core::Resource::Clear()
{
#ifdef _DEBUG
	mReferenceCounts.clear();
#endif
}
