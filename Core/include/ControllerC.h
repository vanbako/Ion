#pragma once
#include "Component.h"

// Controller Component

namespace Ion
{
	namespace Core
	{
		class ControllerC
			: public Core::Component
		{
		public:
			explicit ControllerC(bool isActive, Core::Object* pObject);
			virtual ~ControllerC() = default;
			ControllerC(const ControllerC& other) = default;
			ControllerC(ControllerC&& other) noexcept = default;
			ControllerC& operator=(const ControllerC& other) = default;
			ControllerC& operator=(ControllerC&& other) noexcept = default;

			virtual void Initialize() override;
			virtual void Update(float delta) override;
		};
	}
}
