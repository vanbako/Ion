#include "pch.h"
#include "TextureResource.h"
#include "Texture.h"
#include "Application.h"

using namespace Ion;

Core::TextureResource::TextureResource(Core::ResourceManager* pResourceManager)
	: Resource(pResourceManager)
	, mpResources{}
{
}

Core::TextureResource::~TextureResource()
{
	for (auto& pResource : mpResources)
		delete pResource.second;
	Clear();
}

Core::Texture* Core::TextureResource::AddTexture(const std::string& name)
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
		it = (mpResources.emplace(name, new Core::Texture{ mpResourceManager->GetApplication(), name })).first;
#ifdef _DEBUG
	IncrementReference(name);
#endif
	Core::Texture* pResource{ (*it).second };
	pResource->Initialize();
	mMutex.unlock();
	return pResource;
}

void Core::TextureResource::RemoveTexture(const std::string& name)
{
#ifdef _DEBUG
	if (mpResources.contains(name))
		DecrementReference(name);
#else
	(name);
#endif
}

void Core::TextureResource::Clear()
{
	Core::Resource::Clear();
	mpResources.clear();
}
