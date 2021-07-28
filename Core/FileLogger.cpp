#include "pch.h"
#include "FileLogger.h"

using namespace Ion;

Core::FileLogger::FileLogger(const std::string& location)
	: Core::Logger(location)
{
	std::ofstream file{ mLocation };
	file.close();
}

void Core::FileLogger::Message(void* pClass, Core::MsgType msgType, const std::string& msg)
{
	std::ofstream file{ mLocation, std::ios::app };
	if (!file.is_open())
		return;
	file
		<< mTypeStrings[int(msgType)] << ' '
		<< typeid(pClass).name() << ' '
		<< msg << std::endl;
	file.close();
	if (msgType == Core::MsgType::Fatal)
		throw std::runtime_error{ typeid(pClass).name() + ' ' + msg };
}
