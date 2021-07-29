#include "pch.h"
#include "Command.h"
#include "ReceiverMC.h"

using namespace Ion;

Core::Command::Command(Core::ReceiverMC* pReceiver)
	: mpReceiver{ pReceiver }
	, mValue{ 0 }
{
}

void Core::Command::SetValue(long long value)
{
	mValue = value;
}
