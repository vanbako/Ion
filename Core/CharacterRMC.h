#pragma once
#include "../Core/ReceiverMC.h"

// Character Model Component

namespace Ion
{
	namespace Core
	{
		class TransformMC;

		class CharacterRMC final
			: public ReceiverMC
		{
		public:
			explicit CharacterRMC(bool isActive, Object* pObject);
			~CharacterRMC();
			CharacterRMC(const CharacterRMC& other) = default;
			CharacterRMC(CharacterRMC&& other) noexcept = default;
			CharacterRMC& operator=(const CharacterRMC& other) = default;
			CharacterRMC& operator=(CharacterRMC&& other) noexcept = default;

			void Initialize() override;
			void Update(float delta) override;
			void Switch() override;
			virtual const std::vector<std::pair<std::string, Command*>>& GetCommands() const override;
			virtual const std::string& GetName() const override;

			virtual void MoveForward() override;
			virtual void MoveBack() override;
			virtual void MoveLeft() override;
			virtual void MoveRight() override;
			virtual void RotateLeft() override;
			virtual void RotateRight() override;
		private:
			static const std::string mName;
			std::vector<std::pair<std::string, Command*>> mCommands;
			TransformMC* mpTransformMC;
			bool
				mMoveForward[2],
				mMoveBack[2],
				mMoveLeft[2],
				mMoveRight[2],
				mRotateLeft[2],
				mRotateRight[2];
		};
	}
}
