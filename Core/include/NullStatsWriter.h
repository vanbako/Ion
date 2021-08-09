#pragma once
#include "StatsWriter.h"

#ifdef ION_STATS

namespace Ion
{
	namespace Core
	{
		class NullStatsWriter final
			: public Core::StatsWriter
		{
		public:
			explicit NullStatsWriter(const std::string& url = ".");
			~NullStatsWriter() = default;
			NullStatsWriter(const NullStatsWriter&) = delete;
			NullStatsWriter(NullStatsWriter&&) = delete;
			NullStatsWriter& operator=(const NullStatsWriter&) = delete;
			NullStatsWriter& operator=(const NullStatsWriter&&) = delete;

			virtual void Write(const char* pThisName, Core::Stat stat, float value);
		};
	}
}

#endif