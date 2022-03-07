#include "pch.h"
#include "Material3DResource.h"
#include "Material3D.h"
#include "Application.h"

using namespace Ion;

Core::Material3DResource::Material3DResource(Core::ResourceManager* pResourceManager)
	: Resource(pResourceManager)
	, mpMaterial3Ds{}
{
}

Core::Material3DResource::~Material3DResource()
{
	for (auto& pResource : mpMaterial3Ds)
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
	auto it{ mpMaterial3Ds.find(name) };
	if (it == mpMaterial3Ds.end())
		it = (mpMaterial3Ds.emplace(name, new Core::Material3D{ mpResourceManager->GetApplication(), name })).first;
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
	if (mpMaterial3Ds.contains(name))
		DecrementReference(name);
#else
	(name);
#endif
}

void Core::Material3DResource::Clear()
{
	Core::Resource::Clear();
	mpMaterial3Ds.clear();
}
