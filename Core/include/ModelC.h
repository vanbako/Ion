#pragma once
#include "Component.h"

// Model Component

namespace Ion
{
	namespace Core
	{
		class ModelC
			: public Core::Component
		{
		public:
			explicit ModelC(bool isActive, Core::Object* pObject);
			virtual ~ModelC() = default;
			ModelC(const ModelC& other) = default;
			ModelC(ModelC&& other) noexcept = default;
			ModelC& operator=(const ModelC& other) = default;
			ModelC& operator=(ModelC&& other) noexcept = default;

			virtual void Update(float) override = 0;
			virtual void Switch() = 0;
		protected:
			int mCurrent;
			bool mHasChanged;
		};
	}
}
