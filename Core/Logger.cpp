#include "pch.h"
#include "Logger.h"

#ifdef ION_LOGGER

using namespace Ion;

const std::vector<std::string> Core::Logger::mTypeStrings{
	"Info",
	"Warning",
	"Fatal"
};

Core::Logger::Logger(const std::string& location)
	: mLocation{ location }
{
}

void Core::Logger::Message(const char* pThisName, Core::MsgType msgType, const std::string& msg)
{
	if (msgType == Core::MsgType::Fatal)
		if (pThisName == nullptr)
			throw std::runtime_error{ msg };
		else
			throw std::runtime_error{ pThisName + ' ' + msg };
}

#endif