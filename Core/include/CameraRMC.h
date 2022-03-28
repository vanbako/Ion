#pragma once
#include "MoveRMC.h"

// Camera Receiver Model Component

namespace Ion
{
	namespace Core
	{
		class Canvas;
		class TransformMC;

		class CameraRMC final
			: public Core::MoveRMC
		{
		public:
			explicit CameraRMC(bool isActive, Core::Object* pObject);
			virtual ~CameraRMC();
			CameraRMC(const CameraRMC& other) = default;
			CameraRMC(CameraRMC&& other) noexcept = default;
			CameraRMC& operator=(const CameraRMC& other) = default;
			CameraRMC& operator=(CameraRMC&& other) noexcept = default;

			virtual void Initialize() override;
			virtual void Update(float delta) override;
			virtual const std::vector<std::pair<std::string, Core::Command*>>& GetCommands() const override;
			virtual const std::string& GetName() const override;

			void SetCanvas(Core::Canvas* pCanvas);
			const DirectX::XMFLOAT4X4& GetView() const;
			const DirectX::XMFLOAT4X4& GetViewProjection() const;
			const DirectX::XMFLOAT4X4& GetViewInverse() const;
			Core::TransformMC* GetTransform();
		private:
			bool mFirst;
			static const float
				mMoveFactor,
				mRotateFactor;
			static const std::string mName;
			std::vector<std::pair<std::string, Core::Command*>> mCommands;

			Core::Canvas* mpCanvas;
			float
				mFarPlane,
				mNearPlane,
				mFOV;
			DirectX::XMFLOAT4X4 mView;
			DirectX::XMFLOAT4X4 mViewProjection;
			DirectX::XMFLOAT4X4 mViewInverse;
			Core::TransformMC* mpTransform;
			WINDOWINFO mWindowInfo;
		};
	}
}
