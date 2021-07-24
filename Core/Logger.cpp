#include "pch.h"
#include "Logger.h"

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
