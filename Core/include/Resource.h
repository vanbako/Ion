#pragma once
#include "MsgType.h"

namespace Ion
{
	namespace Core
	{
		class ResourceManager;

		class Resource
		{
		public:
			explicit Resource(Core::ResourceManager* pResourceManager);
			virtual ~Resource() = default;
			Resource(const Resource& other) = delete;
			Resource(Resource&& other) noexcept = delete;
			Resource& operator=(const Resource& other) = delete;
			Resource& operator=(Resource&& other) noexcept = delete;
		protected:
			const std::chrono::milliseconds mMutexDuration;

			Core::ResourceManager* mpResourceManager;
			std::shared_timed_mutex mMutex;
			bool mDeleteOnZero;
#ifdef _DEBUG
			std::unordered_map<std::string, std::size_t> mReferenceCounts;

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
#endif
			virtual void Clear();
		};
	}
}
