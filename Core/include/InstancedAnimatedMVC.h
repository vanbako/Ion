#pragma once
#include "ModelVC.h"
#include "InstanceBuffer.h"
#include "TransformMC.h"
#include "AnimationClip.h"
#include "BonesConstantBuffer.h"

// Instanced Animated Model View Component

namespace Ion
{
	namespace Core
	{
		class Object;
		class Material3D;
		class Canvas3D;
		class Model;
		class Texture;
		class InstancedTransformMC;

		class InstancedAnimatedMVC final
			: public Core::ModelVC
		{
		public:
			explicit InstancedAnimatedMVC(const std::string& modelName, const std::string& modelExtension, const std::string& materialName, bool isActive, Core::Winding winding, Core::CoordSystem coordSystem, Core::Object* pObject);
			virtual ~InstancedAnimatedMVC() = default;
			InstancedAnimatedMVC(const InstancedAnimatedMVC& other) = default;
			InstancedAnimatedMVC(InstancedAnimatedMVC&& other) noexcept = default;
			InstancedAnimatedMVC& operator=(const InstancedAnimatedMVC& other) = default;
			InstancedAnimatedMVC& operator=(InstancedAnimatedMVC&& other) noexcept = default;

			//void AddInstance(const Core::TransformMC& transform);
			//void AddInstances(const std::vector<Core::TransformMC>& transforms);
			void SetInstancedTransform(InstancedTransformMC* pInstancedTransform);

			void SetAnimation(const Core::AnimationClip& animationClip);
			void SetAnimation(size_t clipNumber);
			void SetIsAnimating(bool isAnimating);

			virtual void Initialize() override;
			virtual void Update(float delta) override;
			virtual void Render(Core::Canvas* pCanvas, Core::Material3D* pMaterial) override;
			virtual void Render(Core::Canvas* pCanvas, Core::Material2D* pMaterial) override { (pCanvas); (pMaterial); };
		private:
			InstancedTransformMC* mpInstancedTransform;

			Microsoft::WRL::ComPtr<ID3D12Resource> mpInstanceBuffer;
			std::vector<Core::InstanceBuffer> mInstanceBufferData;
			UINT8* mpInstanceDataBegin;

			std::vector<DirectX::XMFLOAT4X4> mBoneTransforms;
			Core::AnimationClip mAnimationClip;
			float
				mTickCount,
				mAnimationSpeed;
			bool
				mIsAnimating,
				mIsClipSet;

			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mpBonesCbvHeap;
			Microsoft::WRL::ComPtr<ID3D12Resource> mpBonesConstantBuffer;
			Core::BonesConstantBuffer* mpBonesConstantBufferData;
			UINT8* mpBonesCbvDataBegin;
		};
	}
}
