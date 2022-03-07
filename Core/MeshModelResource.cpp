#include "pch.h"
#include "MeshModelResource.h"
#include "MeshModel.h"
#include "Application.h"

using namespace Ion;

Core::MeshModelResource::MeshModelResource(Core::ResourceManager* pResourceManager)
	: Resource(pResourceManager)
	, mpMeshModels{}
{
}

Core::MeshModelResource::~MeshModelResource()
{
	for (auto& pResource : mpMeshModels)
		delete pResource.second;
	Clear();
}

Core::MeshModel* Core::MeshModelResource::AddMeshModel(const std::string& name, const std::string& fileExtension, Core::Winding winding, Core::CoordSystem coordSystem)
{
	if (!mMutex.try_lock_for(mMutexDuration))
	{
#ifdef ION_LOGGER
		mpResourceManager->GetApplication()->GetServiceLocator().GetLogger()->Message(typeid(this).name(), Core::MsgType::Fatal, "Core::DerivedResource::AddResource failed, could not get lock");
#endif
		return nullptr;
	}
	auto it{ mpMeshModels.find(name) };
	if (it == mpMeshModels.end())
		it = (mpMeshModels.emplace(name, new Core::MeshModel{ mpResourceManager->GetApplication(), name, fileExtension, winding, coordSystem })).first;
#ifdef _DEBUG
	IncrementReference(name);
#endif
	Core::MeshModel* pResource{ (*it).second };
	pResource->Initialize();
	mMutex.unlock();
	return pResource;
}

void Core::MeshModelResource::RemoveModel(const std::string& name)
{
#ifdef _DEBUG
	if (mpMeshModels.contains(name))
		DecrementReference(name);
#else
	(name);
#endif
}

void Core::MeshModelResource::Clear()
{
	Core::Resource::Clear();
	mpMeshModels.clear();
}
