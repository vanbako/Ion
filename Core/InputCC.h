#pragma once
#include "../Core/ControllerC.h"

// Input Controller Component

namespace Ion
{
	namespace Core
	{
		class Command;

		class InputCC final
			: public ControllerC
		{
		public:
			explicit InputCC(bool isActive, Object* pObject);
			virtual ~InputCC() = default;
			InputCC(const InputCC& other) = default;
			InputCC(InputCC&& other) noexcept = default;
			InputCC& operator=(const InputCC& other) = default;
			InputCC& operator=(InputCC&& other) noexcept = default;

			virtual void Update(float) override;

			void Queue(Command* pCommand);
		private:
			// No need for thread-safety, Queue & Update are called from the same thread
			std::vector<Command*> mpCommands;
		};
	}
}
