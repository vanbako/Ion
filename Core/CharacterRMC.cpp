#include "pch.h"
#include "CharacterRMC.h"
#include "TransformMC.h"
#include "Object.h"
#include "MoveForwardCmd.h"
#include "MoveBackCmd.h"
#include "MoveLeftCmd.h"
#include "MoveRightCmd.h"
#include "RotateLeftCmd.h"
#include "RotateRightCmd.h"

using namespace Ion;

const std::string Core::CharacterRMC::mName{ "Character" };

Core::CharacterRMC::CharacterRMC(bool isActive, Core::Object* pObject)
	: Core::MoveRMC(isActive, pObject)
	, mCommands{
		{ "MoveForward", new Core::MoveForwardCmd{ this } },
		{ "MoveBack", new Core::MoveBackCmd{ this } },
		{ "MoveLeft", new Core::MoveLeftCmd{ this } },
		{ "MoveRight", new Core::MoveRightCmd{ this } },
		{ "RotateLeft", new Core::RotateLeftCmd{ this } },
		{ "RotateRight", new Core::RotateRightCmd{ this } } }
	, mpTransformMC{ pObject->GetModelC<Core::TransformMC>() }
{
}

Core::CharacterRMC::~CharacterRMC()
{
	for (std::pair<std::string, Core::Command*>& command : mCommands)
		delete command.second;
}

void Core::CharacterRMC::Initialize()
{
	if (mpTransformMC == nullptr)
		mpTransformMC = mpObject->GetModelC<Core::TransformMC>();
	Core::ModelC::Initialize();
}

void Core::CharacterRMC::Update(float delta)
{
	(delta);
	if (!mIsActive)
		return;
	Core::ModelC::Update(delta);
	if (mpTransformMC == nullptr)
		return;
	if (!mHasChanged)
		return;
}

const std::vector<std::pair<std::string, Core::Command*>>& Core::CharacterRMC::GetCommands() const
{
	return mCommands;
}

const std::string& Core::CharacterRMC::GetName() const
{
	return mName;
}
