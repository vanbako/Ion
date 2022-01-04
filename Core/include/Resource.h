#pragma once

#include "MsgType.h"
#include "Model.h"
#include "Texture.h"
#include "Material2D.h"
#include "Material3D.h"

namespace Ion
{
	namespace Core
	{
		class Application;

		template<class T>
		class Resource final
		{
		public:
			explicit Resource(Application* pApplication)
				: mpApplication{ pApplication }
				, mMutexDuration{ 100 }
				, mMutex{}
				, mDeleteOnZero{ false }
				, mReferenceCounts{}
				, mResources{}
			{
			}
			virtual ~Resource() {}
			Resource(const Resource& other) = delete;
			Resource(Resource&& other) noexcept = delete;
			Resource& operator=(const Resource& other) = delete;
			Resource& operator=(Resource&& other) noexcept = delete;

			void Clear()
			{
				// Debug mode: check for ReferenceCount 0?
				mReferenceCounts.clear();
				mResources.clear();
			}

			T* AddResource(const std::string& name)
			{
				if (!mMutex.try_lock_for(mMutexDuration))
				{
#ifdef ION_LOGGER
					mpApplication->GetServiceLocator().GetLogger()->Message(typeid(this).name(), Core::MsgType::Fatal, "Core::Resource::AddResource failed, could not get lock");
#endif
					return nullptr;
				}
				auto ret{ mResources.try_emplace(name, mpApplication, name) };
				IncrementReference(name);
				T* pResource{ &((*(ret.first)).second) };
				pResource->Initialize();
				mMutex.unlock();
				return pResource;
			}
			Core::Model* AddResource(const std::string& name, const std::string& fileExtension, Winding winding, CoordSystem coordSystem)
			{
				if (!mMutex.try_lock_for(mMutexDuration))
				{
#ifdef ION_LOGGER
					mpApplication->GetServiceLocator().GetLogger()->Message(typeid(this).name(), Core::MsgType::Fatal, "Core::Resource::AddResource failed, could not get lock");
#endif
					return nullptr;
				}
				auto ret{ mResources.try_emplace(name, mpApplication, name, fileExtension, winding, coordSystem) };
				IncrementReference(name);
				Core::Model* pResource{ &((*(ret.first)).second) };
				pResource->Initialize();
				mMutex.unlock();
				return pResource;
			}
			void RemoveResource(const std::string& name)
			{
				if (mResources.contains(name))
					DecrementReference(name);
			}
		private:
			const std::chrono::milliseconds mMutexDuration;

			Application* mpApplication;
			std::shared_timed_mutex mMutex;
			bool mDeleteOnZero;
			std::map<std::string, std::size_t> mReferenceCounts;
			std::map<std::string, T> mResources;

			void IncrementReference(const std::string& name)
			{
				auto ret{ mReferenceCounts.find(name) };
				if (ret == mReferenceCounts.end())
					mReferenceCounts.insert({ name, 1 });
				else
					++mReferenceCounts.at(name);
			}
			void DecrementReference(const std::string& name)
			{
				auto ret{ mReferenceCounts.find(name) };
				if (ret != mReferenceCounts.end())
					--mReferenceCounts.at(name);
			}
		};
	}
}
