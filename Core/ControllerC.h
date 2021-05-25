#pragma once
#include "../Core/Component.h"

// Controller Component

namespace Ion
{
	namespace Core
	{
		class ControllerC
			: public Component
		{
		public:
			explicit ControllerC(bool isActive, Object* pObject);
			virtual ~ControllerC() = default;
			ControllerC(const ControllerC& other) = default;
			ControllerC(ControllerC&& other) noexcept = default;
			ControllerC& operator=(const ControllerC& other) = default;
			ControllerC& operator=(ControllerC&& other) noexcept = default;

			virtual void Update(float) override = 0;
		};
	}
}
