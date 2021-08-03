#pragma once
#include "ControllerC.h"

// Input Controller Component

namespace Ion
{
	namespace Core
	{
		class Command;

		class InputCC final
			: public Core::ControllerC
		{
		public:
			explicit InputCC(bool isActive, Core::Object* pObject);
			virtual ~InputCC() = default;
			InputCC(const InputCC& other) = default;
			InputCC(InputCC&& other) noexcept = default;
			InputCC& operator=(const InputCC& other) = default;
			InputCC& operator=(InputCC&& other) noexcept = default;

			virtual void Update(float delta) override;

			void Queue(Core::Command* pCommand);
		private:
			// No need for thread-safety, Queue & Update are called from the same thread
			std::vector<Core::Command*> mpCommands;
		};
	}
}
