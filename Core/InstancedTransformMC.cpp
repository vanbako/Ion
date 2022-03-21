#include "pch.h"
#include "InstancedTransformMC.h"
#include "Application.h"
#include "Material3D.h"
#include "Object.h"
#include "Scene.h"

using namespace Ion;

const std::size_t Core::InstancedTransformMC::mMaxInstances{ 40960 };

Core::InstancedTransformMC::InstancedTransformMC(bool isActive, Core::Object* pObject)
	: ModelC(isActive, pObject)
	, mIsStatic{ true }
	, mHasBehaviour{ false }
	, mTransforms{}
	, mBehaviours{}
{
}

void Core::InstancedTransformMC::AddInstance(const Core::TransformMC& transform, Behaviour behaviour)
{
	mTransforms.emplace_back(transform).Update(0.f);
	if (mHasBehaviour)
		mBehaviours.emplace_back(behaviour);
}

void Core::InstancedTransformMC::AddInstances(const std::vector<Core::TransformMC>& transforms, const std::vector<Core::Behaviour>& behaviours)
{
	if (mHasBehaviour)
	{
		auto it{ behaviours.begin() };
		for (auto& transform : transforms)
		{
			AddInstance(transform, *it);
			++it;
		}
	}
	else
		for (auto& transform : transforms)
			AddInstance(transform, Core::Behaviour::Idle);
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

std::vector<Core::Behaviour>& Core::InstancedTransformMC::GetBehaviours()
{
	return mBehaviours;
}

void Core::InstancedTransformMC::Initialize()
{
	Core::ModelC::Initialize();
}

void Core::InstancedTransformMC::Update(float delta)
{
	if (!mIsActive || mIsStatic)
		return;
	Core::ModelC::Update(delta);
	for (Core::TransformMC& transform : mTransforms)
		transform.Update(delta);
}

void Core::InstancedTransformMC::Switch()
{
	if (!mIsActive || mIsStatic)
		return;
	for (auto& transform : mTransforms)
		transform.Switch();
}

bool Core::InstancedTransformMC::GetIsStatic()
{
	return mIsStatic;
}

void Core::InstancedTransformMC::SetIsStatic(bool isStatic)
{
	mIsStatic = isStatic;
}

void Core::InstancedTransformMC::SetHasBehaviour(bool hasBehaviour)
{
	if (mTransforms.empty())
		mHasBehaviour = hasBehaviour;
}

void Core::InstancedTransformMC::ApplyTerrain(Core::TerrainVC* pTerrainVC)
{
	for (Core::TransformMC& transform : mTransforms)
		transform.ApplyTerrain(pTerrainVC);
}

const std::size_t Core::InstancedTransformMC::GetMaxInstances()
{
	return mMaxInstances;
}
