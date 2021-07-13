#include "pch.h"
#include "InputCC.h"
#include "Command.h"

using namespace Ion::Core;

InputCC::InputCC(bool isActive, Object* pObject)
	: ControllerC(isActive, pObject)
{
}

void InputCC::Update(float)
{
	for (auto pCommand : mpCommands)
	{
		pCommand->Execute();
		delete pCommand;
	}
	mpCommands.clear();
}

void InputCC::Queue(Command* pCommand)
{
	mpCommands.emplace_back(pCommand->Duplicate());
}
