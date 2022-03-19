#include "pch.h"
#include "Material.h"
#include "Application.h"
#include "Canvas.h"

using namespace Ion;

Core::Material::Material(Core::Application* pApplication, const std::string& name)
	: mIsInitialized{ false }
	, mpApplication{ pApplication }
	, mName{ name }
{
}

void Core::Material::Initialize()
{
	mIsInitialized = true;
}
