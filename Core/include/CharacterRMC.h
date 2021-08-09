#pragma once
#include "MoveRMC.h"

// Character Receiver Model Component

namespace Ion
{
	namespace Core
	{
		class TransformMC;

		class CharacterRMC final
			: public Core::MoveRMC
		{
		public:
			explicit CharacterRMC(bool isActive, Core::Object* pObject);
			~CharacterRMC();
			CharacterRMC(const CharacterRMC& other) = default;
			CharacterRMC(CharacterRMC&& other) noexcept = default;
			CharacterRMC& operator=(const CharacterRMC& other) = default;
			CharacterRMC& operator=(CharacterRMC&& other) noexcept = default;

			void Initialize() override;
			void Update(float delta) override;
			virtual const std::vector<std::pair<std::string, Core::Command*>>& GetCommands() const override;
			virtual const std::string& GetName() const override;

			virtual void MoveForward(long long value) override;
			virtual void MoveBack(long long value) override;
			virtual void MoveLeft(long long value) override;
			virtual void MoveRight(long long value) override;
			virtual void MoveUp(long long value) override { (value); };
			virtual void MoveDown(long long value) override { (value); };
			virtual void RotateLeft(long long value) override;
			virtual void RotateRight(long long value) override;
		private:
			static const std::string mName;
			std::vector<std::pair<std::string, Core::Command*>> mCommands;
			Core::TransformMC* mpTransformMC;
		};
	}
}
