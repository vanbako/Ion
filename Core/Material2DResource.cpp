#include "pch.h"
#include "Material2DResource.h"
#include "Material2D.h"
#include "Application.h"

using namespace Ion;

Core::Material2DResource::Material2DResource(Core::ResourceManager* pResourceManager)
	: Resource(pResourceManager)
	, mpResources{}
{
}

Core::Material2DResource::~Material2DResource()
{
	for (auto& pResource : mpResources)
		delete pResource.second;
	Clear();
}

Core::Material2D* Core::Material2DResource::AddMaterial2D(const std::string& name)
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
		it = (mpResources.emplace(name, new Core::Material2D{ mpResourceManager->GetApplication(), name })).first;
#ifdef _DEBUG
	IncrementReference(name);
#endif
	Core::Material2D* pResource{ (*it).second };
	pResource->Initialize();
	mMutex.unlock();
	return pResource;
}

void Core::Material2DResource::RemoveMaterial2D(const std::string& name)
{
#ifdef _DEBUG
	if (mpResources.contains(name))
		DecrementReference(name);
#else
	(name);
#endif
}

void Core::Material2DResource::Clear()
{
	Core::Resource::Clear();
	mpResources.clear();
}
