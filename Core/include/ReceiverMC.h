#pragma once
#include "ModelC.h"

// Receiver Model Component

namespace Ion
{
	namespace Core
	{
		class Command;

		class ReceiverMC
			: public Core::ModelC
		{
		public:
			explicit ReceiverMC(bool isActive, Core::Object* pObject);
			virtual ~ReceiverMC() = default;
			ReceiverMC(const ReceiverMC& other) = default;
			ReceiverMC(ReceiverMC&& other) noexcept = default;
			ReceiverMC& operator=(const ReceiverMC& other) = default;
			ReceiverMC& operator=(ReceiverMC&& other) noexcept = default;

			virtual void Update(float) override = 0;
			virtual void Switch() = 0;
			virtual const std::vector<std::pair<std::string, Core::Command*>>& GetCommands() const = 0;
			virtual const std::string& GetName() const = 0;

			virtual void MoveForward() {};
			virtual void MoveBack() {};
			virtual void MoveLeft() {};
			virtual void MoveRight() {};
			virtual void MoveUp() {};
			virtual void MoveDown() {};
			virtual void RotateLeft() {};
			virtual void RotateRight() {};
			virtual void NextObject() {};
			virtual void PrevObject() {};
			virtual void ShowControls() {};
		};
	}
}
