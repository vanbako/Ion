#include "pch.h"
#include "FileLogger.h"

using namespace Ion::Core;

FileLogger::FileLogger(const std::string& location)
	: Logger(location)
{
}

void FileLogger::Message(void* pClass, MsgType msgType, const std::string& msg)
{
	std::ofstream file{ mLocation };
	if (!file.is_open())
		return;
	file
		<< mTypeStrings[int(msgType)] << ' '
		<< typeid(pClass).name() << ' '
		<< msg << std::endl;
	file.close();
	if (msgType == MsgType::Fatal)
		throw std::runtime_error{ typeid(pClass).name() + ' ' + msg };
}
