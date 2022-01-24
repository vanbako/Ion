#pragma once
#include "Resource.h"

namespace Ion
{
	namespace Core
	{
		class Application;

		class ResourceManager final
		{
		public:
			explicit ResourceManager(Application* pApplication);
			~ResourceManager();
			ResourceManager(const ResourceManager& other) = delete;
			ResourceManager(ResourceManager&& other) noexcept = delete;
			ResourceManager& operator=(const ResourceManager& other) = delete;
			ResourceManager& operator=(ResourceManager&& other) noexcept = delete;

			template<class T>
			T* AddResource()
			{
				std::size_t hashCode{ typeid(T).hash_code() };
				auto it{ mpResources.find(hashCode) };
				if (it == mpResources.end())
					it = (mpResources.emplace(hashCode, new T{ this })).first;
				return (T*)((*it).second);
			}
			template<class T>
			T* GetResource()
			{
				std::size_t hashCode{ typeid(T).hash_code() };
				auto it{ mpResources.find(hashCode) };
				if (it == mpResources.end())
					return nullptr;
				return (T*)((*it).second);
			}
			void Clear();
			Core::Application* GetApplication();
		private:
			Core::Application* mpApplication;
			std::map<std::size_t, Resource*> mpResources;
		};
	}
}
