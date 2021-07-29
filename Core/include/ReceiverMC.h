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

			virtual void MoveForward(long long value) { (value); };
			virtual void MoveBack(long long value) { (value); };
			virtual void MoveLeft(long long value) { (value); };
			virtual void MoveRight(long long value) { (value); };
			virtual void MoveUp(long long value) { (value); };
			virtual void MoveDown(long long value) { (value); };
			virtual void RotateLeft(long long value) { (value); };
			virtual void RotateRight(long long value) { (value); };
			virtual void NextObject(long long value) { (value); };
			virtual void PrevObject(long long value) { (value); };
			virtual void ShowControls(long long value) { (value); };
		};
	}
}
