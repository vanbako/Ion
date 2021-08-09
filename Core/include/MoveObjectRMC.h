#pragma once
#include "MoveRMC.h"

// Camera Receiver Model Component

namespace Ion
{
	namespace Core
	{
		class Canvas;
		class TransformMC;

		class MoveObjectRMC final
			: public Core::MoveRMC
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
			virtual const std::vector<std::pair<std::string, Core::Command*>>& GetCommands() const override;
			virtual const std::string& GetName() const override;

			void SetTransformMC(TransformMC* pTransform);
		private:
			static const std::string mName;
			std::vector<std::pair<std::string, Core::Command*>> mCommands;

			Core::TransformMC* mpTransform;
		};
	}
}
