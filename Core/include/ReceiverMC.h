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

			virtual void Switch() = 0;
			virtual const std::vector<std::pair<std::string, Core::Command*>>& GetCommands() const = 0;
			virtual const std::string& GetName() const = 0;
		};
	}
}
