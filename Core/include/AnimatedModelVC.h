#pragma once
#include "ModelVC.h"
#include "InstanceBuffer.h"
#include "TransformMC.h"
#include "AnimationClip.h"
#include "BonesConstantBuffer.h"

// Animated Model View Component

namespace Ion
{
	namespace Core
	{
		class Object;
		class Material3D;
		class Canvas3D;
		class Model;
		class Texture;

		class AnimatedModelVC
			: public ModelVC
		{
		public:
			explicit AnimatedModelVC(const std::string& modelName, const std::string& materialName, bool isActive, Winding winding, CoordSystem coordSystem, Object* pObject);
			virtual ~AnimatedModelVC();
			AnimatedModelVC(const AnimatedModelVC& other) = default;
			AnimatedModelVC(AnimatedModelVC&& other) noexcept = default;
			AnimatedModelVC& operator=(const AnimatedModelVC& other) = default;
			AnimatedModelVC& operator=(AnimatedModelVC&& other) noexcept = default;

			void SetAnimation(const AnimationClip& animationClip);
			void SetAnimation(size_t clipNumber);
			void SetIsAnimating(bool isAnimating);

			virtual void Initialize() override;
			virtual void Update(float delta) override;
			virtual void Render(Canvas* pCanvas, Material3D* pMaterial) override;
			virtual void Render(Canvas* pCanvas, Material2D* pMaterial) override { (pCanvas); (pMaterial); };
		private:
			std::vector<DirectX::XMFLOAT4X4> mTransforms;
			AnimationClip mAnimationClip;
			float
				mTickCount,
				mAnimationSpeed;
			bool
				mIsAnimating,
				mIsClipSet;

			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mpBonesCbvHeap;
			Microsoft::WRL::ComPtr<ID3D12Resource> mpBonesConstantBuffer;
			BonesConstantBuffer* mpBonesConstantBufferData;
			UINT8* mpBonesCbvDataBegin;
		};
	}
}
