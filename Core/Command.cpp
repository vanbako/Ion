#include "pch.h"
#include "Command.h"
#include "ReceiverMC.h"

using namespace Ion;

Core::Command::Command()
	: mValue{ 0 }
{
}

void Core::Command::SetValue(long long value)
{
	mValue = value;
}
