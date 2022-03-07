#pragma once

#ifdef ION_STATS
#include "SceneThread.h"

namespace Ion
{
	namespace Core
	{
		class StatsST final
			: public Core::SceneThread
		{
		public:
			explicit StatsST(Core::Scene* pScene, std::chrono::milliseconds updateTime);
			virtual ~StatsST();
			StatsST(const StatsST& other) = delete;
			StatsST(StatsST&& other) noexcept = delete;
			StatsST& operator=(const StatsST& other) = delete;
			StatsST& operator=(StatsST&& other) noexcept = delete;
		protected:
			virtual void Inner(float delta) override;
		private:
			void Stats(std::pair<Core::SceneThread*, std::ofstream>& pSceneThread);

			std::unordered_map<std::string, std::pair<Core::SceneThread*, std::ofstream>> mpSceneThreads;
			std::chrono::milliseconds mStats[mStatMax];
			std::size_t mStatSeqs[mStatMax];
		};
	}
}
#endif
