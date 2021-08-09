#pragma once

#ifdef ION_STATS

#include "Stat.h"

namespace Ion
{
	namespace Core
	{
		class StatsWriter
		{
		public:
			explicit StatsWriter(const std::string& url);
			virtual ~StatsWriter() = default;
			StatsWriter(const StatsWriter&) = delete;
			StatsWriter(StatsWriter&&) = delete;
			StatsWriter& operator=(const StatsWriter&) = delete;
			StatsWriter& operator=(StatsWriter&&) = delete;

			virtual void Write(const char* pThisName, Core::Stat stat, float value) = 0;
		protected:
			const std::string mUrl;
		};
	}
}

#endif