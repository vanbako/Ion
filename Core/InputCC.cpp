#include "pch.h"
#include "InputCC.h"
#include "Command.h"

using namespace Ion;

Core::InputCC::InputCC(bool isActive, Core::Object* pObject)
	: Core::ControllerC(isActive, pObject)
{
}

void Core::InputCC::Update(float delta)
{
	(delta);
	for (auto pCommand : mpCommands)
	{
		pCommand->Execute();
		delete pCommand;
	}
	mpCommands.clear();
}

void Core::InputCC::Queue(Core::Command* pCommand)
{
	mpCommands.emplace_back(pCommand);
}
