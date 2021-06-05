#pragma once
#include "../Core/ModelC.h"

// Receiver Model Component

namespace Ion
{
	namespace Core
	{
		class Command;

		class ReceiverMC
			: public ModelC
		{
		public:
			explicit ReceiverMC(bool isActive, Object* pObject);
			virtual ~ReceiverMC() = default;
			ReceiverMC(const ReceiverMC& other) = default;
			ReceiverMC(ReceiverMC&& other) noexcept = default;
			ReceiverMC& operator=(const ReceiverMC& other) = default;
			ReceiverMC& operator=(ReceiverMC&& other) noexcept = default;

			virtual void Update(float) override = 0;
			virtual void Switch() = 0;
			virtual const std::vector<std::pair<std::string, Command*>>& GetCommands() const = 0;
			virtual const std::string& GetName() const = 0;

			virtual void MoveForward() = 0;
			virtual void MoveBack() = 0;
			virtual void MoveLeft() = 0;
			virtual void MoveRight() = 0;
			virtual void MoveUp() = 0;
			virtual void MoveDown() = 0;
			virtual void RotateLeft() = 0;
			virtual void RotateRight() = 0;
		};
	}
}
