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

		class InstancedAnimatedModelVC
			: public Core::ModelVC
		{
		public:
			explicit InstancedAnimatedModelVC(const std::string& modelName, const std::string& modelExtension, const std::string& materialName, bool isActive, Core::Winding winding, Core::CoordSystem coordSystem, Core::Object* pObject);
			virtual ~InstancedAnimatedModelVC();
			InstancedAnimatedModelVC(const InstancedAnimatedModelVC& other) = default;
			InstancedAnimatedModelVC(InstancedAnimatedModelVC&& other) noexcept = default;
			InstancedAnimatedModelVC& operator=(const InstancedAnimatedModelVC& other) = default;
			InstancedAnimatedModelVC& operator=(InstancedAnimatedModelVC&& other) noexcept = default;

			void AddInstance(const Core::TransformMC& transform);
			void AddInstances(const std::vector<Core::TransformMC>& transforms);
			void ReadInstances();
			std::vector<Core::TransformMC>& GetInstances();

			void SetAnimation(const Core::AnimationClip& animationClip);
			void SetAnimation(size_t clipNumber);
			void SetIsAnimating(bool isAnimating);

			virtual void Initialize() override;
			virtual void Update(float delta) override;
			virtual void Render(Core::Canvas* pCanvas, Core::Material3D* pMaterial) override;
			virtual void Render(Core::Canvas* pCanvas, Core::Material2D* pMaterial) override { (pCanvas); (pMaterial); };
		private:
			static const size_t mMaxInstances;
			std::vector<Core::TransformMC> mTransforms;

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
