#include "pch.h"
#include "InstancedTransformMC.h"
#include "Application.h"
#include "Material3D.h"
#include "Object.h"
#include "Scene.h"

using namespace Ion;

const size_t Core::InstancedTransformMC::mMaxInstances{ 40960 };

Core::InstancedTransformMC::InstancedTransformMC(bool isActive, Core::Object* pObject)
	: ModelC(isActive, pObject)
	, mIsStatic{ true }
	, mTransforms{}
{
}

void Core::InstancedTransformMC::AddInstance(const Core::TransformMC& transform)
{
	mTransforms.push_back(transform);
	mTransforms.back().Update(0.f);
}

void Core::InstancedTransformMC::AddInstances(const std::vector<Core::TransformMC>& transforms)
{
	for (auto& transform : transforms)
		AddInstance(transform);
}

//void Core::InstancedTransformMC::ReadInstances()
//{
//	if (!mTransforms.empty())
//		return;
//	const std::vector<Core::Transform>& transforms{ mpModel->ReadInstances() };
//	Core::TransformMC transformMC{ true, mpObject };
//	for (auto& transform : transforms)
//	{
//		transformMC.SetPosition(transform.mPosition);
//		transformMC.SetScale(transform.mScale);
//		transformMC.SetRotation(transform.mRotation);
//		AddInstance(transformMC);
//	}
//}

std::vector<Core::TransformMC>& Core::InstancedTransformMC::GetInstances()
{
	return mTransforms;
}

void Core::InstancedTransformMC::Initialize()
{
	mIsInitialized = true;
}

void Core::InstancedTransformMC::Update(float delta)
{
	if (!mIsActive || mIsStatic)
		return;
	for (auto& transform : mTransforms)
		transform.Update(delta);
}

void Core::InstancedTransformMC::Switch()
{
	if (!mIsActive || mIsStatic)
		return;
	for (auto& transform : mTransforms)
		transform.Switch();
}

void Core::InstancedTransformMC::SetIsStatic(bool isStatic)
{
	mIsStatic = isStatic;
}

const size_t Core::InstancedTransformMC::GetMaxInstances()
{
	return mMaxInstances;
}
