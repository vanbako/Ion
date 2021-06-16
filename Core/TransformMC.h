#pragma once
#include "../Core/ModelC.h"
#include "../Core/AngleUnit.h"

// Transform Model Component

namespace Ion
{
	namespace Core
	{
		class TransformMC final
			: public ModelC
		{
		public:
			explicit TransformMC(bool isActive, Object* pObject);
			~TransformMC() = default;
			TransformMC(const TransformMC& other) = default;
			TransformMC(TransformMC&& other) noexcept = default;
			TransformMC& operator=(const TransformMC& other) = default;
			TransformMC& operator=(TransformMC&& other) noexcept = default;

			void Initialize() override;
			void Update(float delta) override;
			void Switch() override;

			void SetPosition(const DirectX::XMFLOAT4& position);
			void SetScale(const DirectX::XMFLOAT4& scale);
			void SetRotation(const DirectX::XMFLOAT4& rotation);
			void SetRotation(const DirectX::XMFLOAT3& rotation, AngleUnit unit = AngleUnit::Degree);
			void SetForward(const DirectX::XMFLOAT4& forward);
			void SetUp(const DirectX::XMFLOAT4& up);
			void SetRight(const DirectX::XMFLOAT4& right);

			const DirectX::XMFLOAT4& GetWorldPosition() const;
			const DirectX::XMFLOAT4& GetForward() const;
			const DirectX::XMFLOAT4& GetUp() const;
			const DirectX::XMFLOAT4& GetRight() const;
			const DirectX::XMFLOAT4X4& GetWorld() const;
		private:
			void InternalUpdate(float delta);
			void InternalSwitch();

			DirectX::XMFLOAT4
				mPosition[2],
				mWorldPosition[2],
				mForward[2],
				mUp[2],
				mRight[2],
				mScale[2],
				mRotation[2];
			DirectX::XMFLOAT4X4 mWorld[2];
		};
	}
}
