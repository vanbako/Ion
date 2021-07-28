#pragma once
#include "ReceiverMC.h"

// Camera Model Component

namespace Ion
{
	namespace Core
	{
		class Canvas;
		class TransformMC;

		class MoveObjectRMC final
			: public Core::ReceiverMC
		{
		public:
			explicit MoveObjectRMC(bool isActive, Core::Object* pObject);
			virtual ~MoveObjectRMC();
			MoveObjectRMC(const MoveObjectRMC& other) = default;
			MoveObjectRMC(MoveObjectRMC&& other) noexcept = default;
			MoveObjectRMC& operator=(const MoveObjectRMC& other) = default;
			MoveObjectRMC& operator=(MoveObjectRMC&& other) noexcept = default;

			virtual void Initialize() override;
			virtual void Update(float delta) override;
			virtual void Switch() override;
			virtual const std::vector<std::pair<std::string, Core::Command*>>& GetCommands() const override;
			virtual const std::string& GetName() const override;

			void SetTransformMC(TransformMC* pTransform);

			virtual void MoveForward() override;
			virtual void MoveBack() override;
			virtual void MoveLeft() override;
			virtual void MoveRight() override;
			virtual void MoveUp() override;
			virtual void MoveDown() override;
			virtual void RotateLeft() override;
			virtual void RotateRight() override;
		private:
			static const std::string mName;
			std::vector<std::pair<std::string, Core::Command*>> mCommands;

			Core::TransformMC* mpTransform;
			// My best guess is that boolean operations are thread-safe
			// Implement std::atomic if needed
			bool
				mMoveForward[2],
				mMoveBack[2],
				mMoveLeft[2],
				mMoveRight[2],
				mMoveUp[2],
				mMoveDown[2],
				mRotateLeft[2],
				mRotateRight[2];
		};
	}
}
