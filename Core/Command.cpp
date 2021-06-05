#include "../Core/pch.h"
#include "../Core/Command.h"
#include "../Core/ReceiverMC.h"

using namespace Ion::Core;

Command::Command(ReceiverMC* pReceiver)
	: mpReceiver{ pReceiver }
{
}
