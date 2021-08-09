#include "pch.h"
#include "FileLogger.h"

#ifdef ION_LOGGER

using namespace Ion;

Core::FileLogger::FileLogger(const std::string& location)
	: Core::Logger(location)
{
	std::ofstream file{ mLocation };
	file.close();
}

void Core::FileLogger::Message(const char* pThisName, Core::MsgType msgType, const std::string& msg)
{
	std::ofstream file{ mLocation, std::ios::app };
	if (!file.is_open())
		return;
	if (pThisName == nullptr)
		file
			<< mTypeStrings[int(msgType)] << ' '
			<< msg << std::endl;
	else
		file
			<< mTypeStrings[int(msgType)] << ' '
			<< pThisName << ' '
			<< msg << std::endl;
	file.close();
	Core::Logger::Message(pThisName, msgType, msg);
}

#endif