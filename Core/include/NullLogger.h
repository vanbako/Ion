#pragma once
#include "Logger.h"

#ifdef ION_LOGGER

namespace Ion
{
	namespace Core
	{
		class NullLogger final
			: public Core::Logger
		{
		public:
			explicit NullLogger(const std::string& location = "");
			~NullLogger() = default;
			NullLogger(const NullLogger&) = delete;
			NullLogger(NullLogger&&) = delete;
			NullLogger& operator=(const NullLogger&) = delete;
			NullLogger& operator=(const NullLogger&&) = delete;
		};
	}
}

#endif