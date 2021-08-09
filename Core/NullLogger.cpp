#include "pch.h"
#include "NullLogger.h"

#ifdef ION_LOGGER

using namespace Ion;

Core::NullLogger::NullLogger(const std::string& location)
	: Core::Logger(location)
{
}

#endif