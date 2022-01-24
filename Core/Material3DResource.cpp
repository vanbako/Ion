#include "pch.h"
#include "Material3DResource.h"
#include "Material3D.h"
#include "Application.h"

using namespace Ion;

Core::Material3DResource::Material3DResource(Core::ResourceManager* pResourceManager)
	: Resource(pResourceManager)
	, mpResources{}
{
}

Core::Material3DResource::~Material3DResource()
{
	for (auto& pResource : mpResources)
		delete pResource.second;
	Clear();
}

Core::Material3D* Core::Material3DResource::AddMaterial3D(const std::string& name)
{
	if (!mMutex.try_lock_for(mMutexDuration))
	{
#ifdef ION_LOGGER
		mpResourceManager->GetApplication()->GetServiceLocator().GetLogger()->Message(typeid(this).name(), Core::MsgType::Fatal, "Core::DerivedResource::AddResource failed, could not get lock");
#endif
		return nullptr;
	}
	auto it{ mpResources.find(name) };
	if (it == mpResources.end())
		it = (mpResources.emplace(name, new Core::Material3D{ mpResourceManager->GetApplication(), name })).first;
#ifdef _DEBUG
	IncrementReference(name);
#endif
	Core::Material3D* pResource{ (*it).second };
	pResource->Initialize();
	mMutex.unlock();
	return pResource;
}

void Core::Material3DResource::RemoveMaterial3D(const std::string& name)
{
#ifdef _DEBUG
	if (mpResources.contains(name))
		DecrementReference(name);
#else
	(name);
#endif
}

void Core::Material3DResource::Clear()
{
	Core::Resource::Clear();
	mpResources.clear();
}
