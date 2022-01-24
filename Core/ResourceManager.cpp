#include "pch.h"
#include "ResourceManager.h"

using namespace Ion;

Core::ResourceManager::ResourceManager(Application* pApplication)
	: mpApplication{ pApplication }
	, mpResources{}
{
}

Core::ResourceManager::~ResourceManager()
{
	Clear();
}

void Core::ResourceManager::Clear()
{
	for (auto& pr : mpResources)
		delete pr.second;
	mpResources.clear();
}

Core::Application* Core::ResourceManager::GetApplication()
{
	return mpApplication;
}
