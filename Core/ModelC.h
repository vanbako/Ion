#pragma once
#include "../Core/Component.h"

// Model Component

namespace Ion
{
	namespace Core
	{
		class ModelC
			: public Component
		{
		public:
			explicit ModelC(bool isActive, Object* pObject);
			virtual ~ModelC() = default;
			ModelC(const ModelC& other) = default;
			ModelC(ModelC&& other) noexcept = default;
			ModelC& operator=(const ModelC& other) = default;
			ModelC& operator=(ModelC&& other) noexcept = default;

			virtual void Update(float) override = 0;
			virtual void Switch() = 0;
		};
	}
}
