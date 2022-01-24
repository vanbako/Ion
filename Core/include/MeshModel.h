#pragma once
#include "AnimationClip.h"
#include "BinIfstream.h"
#include "InputSemantic.h"
#include "Winding.h"
#include "CoordSystem.h"
#include "Transform.h"
#include "Int4.h"

namespace Ion
{
	namespace Core
	{
		class Application;

		class MeshModel final
		{
		public:
			explicit MeshModel(Core::Application* pApplication, const std::string& fileName, const std::string& fileExtension, Core::Winding winding, Core::CoordSystem coordSystem);
			~MeshModel() = default;
			MeshModel(const MeshModel& other) = delete;
			MeshModel(MeshModel&& other) noexcept = delete;
			MeshModel& operator=(const MeshModel& other) = delete;
			MeshModel& operator=(MeshModel&& other) noexcept = delete;

			void Initialize();
			const std::vector<DWORD>& GetIndices() const;
			const std::vector<DirectX::XMFLOAT3>& GetPositions() const;
			const std::vector<DirectX::XMFLOAT3>& GetNormals() const;
			const std::vector<DirectX::XMFLOAT3>& GetTangents() const;
			const std::vector<DirectX::XMFLOAT3>& GetBinormals() const;
			const std::vector<DirectX::XMFLOAT2>& GetTexCoords() const;
			const std::vector<DirectX::XMFLOAT4>& GetColors() const;
			const std::vector<Core::Int4>& GetBlendIndices() const;
			const std::vector<DirectX::XMFLOAT4>& GetBlendWeights() const;
			const std::vector<Core::AnimationClip>& GetAnimationClips() const;
			std::size_t GetElementCount();
			bool HasInputElem(Core::InputSemantic inputSemantic);
			bool HasInputElem(const std::string& inputSemantic);
			const std::vector<Core::Transform>& ReadInstances();
			const std::vector<Core::Transform>& GetInstances() const;
		private:
			bool mIsInitialized;
			Core::Application* mpApplication;
			std::string mFileName;
			std::string mFileExtension;
			Core::Winding mWinding;
			Core::CoordSystem mCoordSystem;
			std::wstring mName;
			std::size_t
				mTexCoordCount,
				mBoneCount;
			std::vector<DWORD> mIndices;
			std::vector<DirectX::XMFLOAT3> mPositions;
			std::vector<DirectX::XMFLOAT3> mNormals;
			std::vector<DirectX::XMFLOAT3> mTangents;
			std::vector<DirectX::XMFLOAT3> mBinormals;
			std::vector<DirectX::XMFLOAT2> mTexCoords;
			std::vector<DirectX::XMFLOAT4> mColors;
			std::vector<Int4> mBlendIndices;
			std::vector<DirectX::XMFLOAT4> mBlendWeights;
			std::vector<Core::AnimationClip> mAnimationClips;
			std::vector<Core::Transform> mInstances;
			std::bitset<std::size_t(Core::InputSemantic::Count)> mElem;
			bool mHasAnimation;

			void ReadModel();
		};
	}
}
