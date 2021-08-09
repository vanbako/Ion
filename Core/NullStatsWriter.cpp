#include "pch.h"
#include "NullStatsWriter.h"

#ifdef ION_STATS

using namespace Ion;

Core::NullStatsWriter::NullStatsWriter(const std::string& url)
	: Core::StatsWriter(url)
{
}

void Core::NullStatsWriter::Write(const char* pThisName, Core::Stat stat, float value)
{
	(pThisName);
	(stat);
	(value);
}

#endif