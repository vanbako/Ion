#pragma once
#include "ModelVC.h"
#include "TransformMC.h"

// Instanced Transform Model Component

namespace Ion
{
	namespace Core
	{
		class Object;

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

			void AddInstance(const Core::TransformMC& transform);
			void AddInstances(const std::vector<Core::TransformMC>& transforms);
			//void ReadInstances();
			std::vector<Core::TransformMC>& GetInstances();

			virtual void Initialize() override;
			virtual void Update(float delta) override;
			virtual void Switch() override;

			void SetIsStatic(bool isStatic);
			static const size_t GetMaxInstances();
		private:
			static const size_t mMaxInstances;
			bool mIsStatic;
			std::vector<Core::TransformMC> mTransforms;
		};
	}
}
