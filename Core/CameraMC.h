#pragma once
#include "../Core/ModelC.h"

// Camera Model Component

namespace Ion
{
	namespace Core
	{
		class Canvas;

		class CameraMC final
			: public ModelC
		{
		public:
			explicit CameraMC(bool isActive, Object* pObject);
			virtual ~CameraMC() = default;
			CameraMC(const CameraMC& other) = default;
			CameraMC(CameraMC&& other) noexcept = default;
			CameraMC& operator=(const CameraMC& other) = default;
			CameraMC& operator=(CameraMC&& other) noexcept = default;

			virtual void Initialize() override;
			virtual void Update(float delta) override;
			virtual void Switch() override;

			void SetCanvas(Canvas* pCanvas);
			const DirectX::XMFLOAT4X4& GetViewProjection();
		private:
			Canvas* mpCanvas;
			float
				mFarPlane,
				mNearPlane,
				mFOV;
			DirectX::XMFLOAT4X4 mViewProjection;
		};
	}
}
