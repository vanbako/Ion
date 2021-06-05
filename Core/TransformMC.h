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

			void SetPosition(const DirectX::XMFLOAT3& position);
			void SetScale(const DirectX::XMFLOAT3& scale);
			void SetRotation(const DirectX::XMFLOAT4& rotation);
			void SetRotation(const DirectX::XMFLOAT3& rotation, AngleUnit unit = AngleUnit::Degree);

			const DirectX::XMFLOAT3& GetWorldPosition() const;
			const DirectX::XMFLOAT3& GetForward() const;
			const DirectX::XMFLOAT3& GetUp() const;
			const DirectX::XMFLOAT4X4& GetWorld() const;
		private:
			void InternalUpdate(float delta);
			void InternalSwitch();

			DirectX::XMFLOAT3
				mPosition[2],
				mWorldPosition[2],
				mForward[2],
				mUp[2],
				mScale[2];
			DirectX::XMFLOAT4 mRotation[2];
			DirectX::XMFLOAT4X4 mWorld[2];
		};
	}
}
