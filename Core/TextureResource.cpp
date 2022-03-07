#include "pch.h"
#include "TextureResource.h"
#include "Texture.h"
#include "Application.h"

using namespace Ion;

Core::TextureResource::TextureResource(Core::ResourceManager* pResourceManager)
	: Resource(pResourceManager)
	, mpTextures{}
{
}

Core::TextureResource::~TextureResource()
{
	for (auto& pResource : mpTextures)
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
	auto it{ mpTextures.find(name) };
	if (it == mpTextures.end())
		it = (mpTextures.emplace(name, new Core::Texture{ mpResourceManager->GetApplication(), name })).first;
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
	if (mpTextures.contains(name))
		DecrementReference(name);
#else
	(name);
#endif
}

void Core::TextureResource::Clear()
{
	Core::Resource::Clear();
	mpTextures.clear();
}
