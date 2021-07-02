#pragma once

#include "../Core/AnimationClip.h"
#include "../Core/BinIfstream.h"
#include "../Core/InputSemantic.h"
#include "../Core/Winding.h"
#include "../Core/Transform.h"

namespace Ion
{
	namespace Core
	{
		class Application;

		class Model final
		{
		public:
			explicit Model(Application* pApplication, const std::string& name, Winding winding);
			~Model() = default;
			Model(const Model& other) = delete;
			Model(Model&& other) noexcept = delete;
			Model& operator=(const Model& other) = delete;
			Model& operator=(Model&& other) noexcept = delete;

			void Initialize();
			const std::vector<DWORD>& GetIndices() const;
			const std::vector<DirectX::XMFLOAT3>& GetPositions() const;
			const std::vector<DirectX::XMFLOAT3>& GetNormals() const;
			const std::vector<DirectX::XMFLOAT3>& GetTangents() const;
			const std::vector<DirectX::XMFLOAT3>& GetBinormals() const;
			const std::vector<DirectX::XMFLOAT2>& GetTexCoords() const;
			const std::vector<DirectX::XMFLOAT4>& GetColors() const;
			const std::vector<DirectX::XMFLOAT4>& GetBlendIndices() const;
			const std::vector<DirectX::XMFLOAT4>& GetBlendWeights() const;
			size_t GetElementCount();
			bool HasInputElem(InputSemantic inputSemantic);
			bool HasInputElem(const std::string& inputSemantic);
			const std::vector<Transform>& ReadInstances();
			const std::vector<Transform>& GetInstances() const;
		private:
			bool mIsInitialized;
			Application* mpApplication;
			std::string mFileName;
			Winding mWinding;
			std::wstring mName;
			size_t
				mTexCoordCount,
				mBoneCount;
			std::vector<DWORD> mIndices;
			std::vector<DirectX::XMFLOAT3> mPositions;
			std::vector<DirectX::XMFLOAT3> mNormals;
			std::vector<DirectX::XMFLOAT3> mTangents;
			std::vector<DirectX::XMFLOAT3> mBinormals;
			std::vector<DirectX::XMFLOAT2> mTexCoords;
			std::vector<DirectX::XMFLOAT4> mColors;
			std::vector<DirectX::XMFLOAT4> mBlendIndices;
			std::vector<DirectX::XMFLOAT4> mBlendWeights;
			std::vector<AnimationClip> mAnimationClips;
			std::vector<Transform> mInstances;
			std::bitset<size_t(InputSemantic::Count)> mElem;
			bool mHasAnimation;

			void ReadModel();
		};
	}
}
