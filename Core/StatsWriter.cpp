#include "pch.h"
#include "StatsWriter.h"

#ifdef ION_STATS

using namespace Ion;

Core::StatsWriter::StatsWriter(const std::string& url)
	: mUrl{ url }
{
}

#endif