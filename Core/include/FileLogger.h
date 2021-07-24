#pragma once
#include "Logger.h"

namespace Ion
{
	namespace Core
	{
		class FileLogger final
			: public Core::Logger
		{
		public:
			explicit FileLogger(const std::string& location);
			~FileLogger() = default;
			FileLogger(const FileLogger&) = delete;
			FileLogger(FileLogger&&) = delete;
			FileLogger& operator=(const FileLogger&) = delete;
			FileLogger& operator=(const FileLogger&&) = delete;

			virtual void Message(void* pClass, Core::MsgType msgType, const std::string& msg) override;
		};
	}
}
