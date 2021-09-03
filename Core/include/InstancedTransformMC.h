#pragma once
#include "ModelVC.h"
#include "TransformMC.h"
#include "Behaviour.h"

// Instanced Transform Model Component

namespace Ion
{
	namespace Core
	{
		class Object;
		class TerrainVC;

		class InstancedTransformMC final
			: public Core::ModelC
		{
		public:
			explicit InstancedTransformMC(bool isActive, Core::Object* pObject);
			virtual ~InstancedTransformMC() = default;
			InstancedTransformMC(const InstancedTransformMC& other) = default;
			InstancedTransformMC(InstancedTransformMC&& other) noexcept = default;
			InstancedTransformMC& operator=(const InstancedTransformMC& other) = default;
			InstancedTransformMC& operator=(InstancedTransformMC&& other) noexcept = default;

			void AddInstance(const Core::TransformMC& transform, Behaviour behaviour);
			void AddInstances(const std::vector<Core::TransformMC>& transforms, const std::vector<Core::Behaviour>& behaviours);
			//void ReadInstances();
			std::vector<Core::TransformMC>& GetInstances();
			std::vector<Core::Behaviour>& GetBehaviours();

			virtual void Initialize() override;
			virtual void Update(float delta) override;
			virtual void Switch() override;

			bool GetIsStatic();
			void SetIsStatic(bool isStatic);
			void SetHasBehaviour(bool hasBehaviour);
			void ApplyTerrain(Core::TerrainVC* pTerrainVC);

			static const size_t GetMaxInstances();
		private:
			static const size_t mMaxInstances;
			bool
				mIsStatic,
				mHasBehaviour;
			std::vector<Core::TransformMC> mTransforms;
			std::vector<Core::Behaviour> mBehaviours;
		};
	}
}
