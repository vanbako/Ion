#include "pch.h"
#include "NullLogger.h"

using namespace Ion;

Core::NullLogger::NullLogger(const std::string& location)
	: Core::Logger(location)
{
}

void Core::NullLogger::Message(void* pClass, Core::MsgType msgType, const std::string& msg)
{
	(pClass);
	(msgType);
	(msg);
}
