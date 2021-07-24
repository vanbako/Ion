#pragma once
#include "MsgType.h"

namespace Ion
{
	namespace Core
	{
		class Logger
		{
		public:
			explicit Logger(const std::string& location);
			virtual ~Logger() = default;
			Logger(const Logger&) = delete;
			Logger(Logger&&) = delete;
			Logger& operator=(const Logger&) = delete;
			Logger& operator=(Logger&&) = delete;

			virtual void Message(void* pClass, Core::MsgType msgType, const std::string& msg) = 0;
		protected:
			const std::string mLocation;

			static const std::vector<std::string> mTypeStrings;
		};
	}
}
