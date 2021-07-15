#include "pch.h"
#include "Model.h"
#include "Application.h"
#include "MeshType.h"
#include "Material3D.h"

using namespace Ion::Core;

Model::Model(Application* pApplication, const std::string& name, Winding winding, CoordSystem coordSystem)
	: mIsInitialized{ false }
	, mpApplication{ pApplication }
	, mFileName{ name }
	, mWinding{ winding }
	, mCoordSystem{ coordSystem }
	, mName{}
	, mTexCoordCount{ 0 }
	, mBoneCount{ 0 }
	, mIndices{}
	, mPositions{}
	, mNormals{}
	, mTangents{}
	, mBinormals{}
	, mTexCoords{}
	, mColors{}
	, mBlendIndices{}
	, mBlendWeights{}
	, mAnimationClips{}
	, mElem{ 0x0 }
	, mHasAnimation{ false }
{
	ReadModel();
}

void Model::Initialize()
{
	if (mIsInitialized)
		return;
	mIsInitialized = true;
}

const std::vector<DWORD>& Model::GetIndices() const
{
	return mIndices;
}

const std::vector<DirectX::XMFLOAT3>& Model::GetPositions() const
{
	return mPositions;
}

const std::vector<DirectX::XMFLOAT3>& Model::GetNormals() const
{
	return mNormals;
}

const std::vector<DirectX::XMFLOAT3>& Model::GetTangents() const
{
	return mTangents;
}

const std::vector<DirectX::XMFLOAT3>& Model::GetBinormals() const
{
	return mBinormals;
}

const std::vector<DirectX::XMFLOAT2>& Model::GetTexCoords() const
{
	return mTexCoords;
}

const std::vector<DirectX::XMFLOAT4>& Model::GetColors() const
{
	return mColors;
}

const std::vector<Int4>& Model::GetBlendIndices() const
{
	return mBlendIndices;
}

const std::vector<DirectX::XMFLOAT4>& Model::GetBlendWeights() const
{
	return mBlendWeights;
}

const std::vector<AnimationClip>& Ion::Core::Model::GetAnimationClips() const
{
	return mAnimationClips;
}

size_t Model::GetElementCount()
{
	size_t cnt{ 0 };
	for (size_t i{ 0 }; i < size_t(InputSemantic::Count); ++i)
		cnt += mElem[i];
	return cnt;
}

bool Model::HasInputElem(InputSemantic inputSemantic)
{
	return (mElem[size_t(inputSemantic)] == 1);
}

bool Model::HasInputElem(const std::string& inputSemantic)
{
	return HasInputElem(Material3D::GetSemanticStrings().at(inputSemantic).inputSemantic);
}

const std::vector<Transform>& Model::ReadInstances()
{
	if (mInstances.empty())
	{
		BinIfstream file{ "../Resources/Instances/" + mFileName + ".ins" };
		size_t cnt{ file.Read<size_t>() };
		for (size_t i{ 0 }; i < cnt; ++i)
			mInstances.emplace_back(file.Read<Transform>());
	}
	return mInstances;
}

const std::vector<Transform>& Model::GetInstances() const
{
	return mInstances;
}

void Model::ReadModel()
{
	using namespace DirectX;
	BinIfstream file{ "../Resources/Model/" + mFileName + ".ovm" };
	const char majorVersion{ file.Read<char>() };
	const char minorVersion{ file.Read<char>() };

	size_t
		indexCount{ 0 },
		vertexCount{ 0 };
	for (;;)
	{
		MeshType meshType{ MeshType(file.Read<char>()) };
		if (meshType == MeshType::End)
			break;
		unsigned int offset{ file.Read<unsigned int>() };

		switch (meshType)
		{
		case MeshType::Header:
			mName = file.ReadString();
			vertexCount = size_t(file.Read<unsigned int>());
			indexCount = size_t(file.Read<unsigned int>());
			break;
		case MeshType::Positions:
			mElem[size_t(InputSemantic::Position)] = 1;
			switch (mCoordSystem)
			{
			case CoordSystem::LeftHanded:
				for (size_t i{ 0 }; i < vertexCount; ++i)
				{
					DirectX::XMFLOAT3 position{};
					position.x = file.Read<float>();
					position.y = file.Read<float>();
					position.z = file.Read<float>();
					mPositions.push_back(position);
				}
				break;
			case CoordSystem::RightHanded:
				for (size_t i{ 0 }; i < vertexCount; ++i)
				{
					DirectX::XMFLOAT3 position{};
					position.x = file.Read<float>();
					position.z = file.Read<float>();
					position.y = -file.Read<float>();
					mPositions.push_back(position);
				}
				break;
			}
			break;
		case MeshType::Indices:
		{
			size_t cnt{ indexCount / 3 };
			switch (mWinding)
			{
			case Winding::CW:
				for (size_t i{ 0 }; i < cnt; ++i)
				{
					mIndices.push_back(file.Read<DWORD>());
					mIndices.push_back(file.Read<DWORD>());
					mIndices.push_back(file.Read<DWORD>());
				}
				break;
			case Winding::CCW:
				DWORD two{};
				for (size_t i{ 0 }; i < cnt; ++i)
				{
					mIndices.push_back(file.Read<DWORD>());
					two = file.Read<DWORD>();
					mIndices.push_back(file.Read<DWORD>());
					mIndices.push_back(two);
				}
				break;
			}
			break;
		}
		case MeshType::Normals:
			mElem[size_t(InputSemantic::Normal)] = 1;
			switch (mCoordSystem)
			{
			case CoordSystem::LeftHanded:
				for (size_t i{ 0 }; i < vertexCount; ++i)
				{
					DirectX::XMFLOAT3 normal{};
					normal.x = file.Read<float>();
					normal.y = file.Read<float>();
					normal.z = file.Read<float>();
					mNormals.push_back(normal);
				}
				break;
			case CoordSystem::RightHanded:
				for (size_t i{ 0 }; i < vertexCount; ++i)
				{
					DirectX::XMFLOAT3 normal{};
					normal.x = file.Read<float>();
					normal.z = file.Read<float>();
					normal.y = -file.Read<float>();
					mNormals.push_back(normal);
				}
				break;
			}
			break;
		case MeshType::Tangents:
			mElem[size_t(InputSemantic::Tangent)] = 1;
			switch (mCoordSystem)
			{
			case CoordSystem::LeftHanded:
				for (size_t i{ 0 }; i < vertexCount; ++i)
				{
					DirectX::XMFLOAT3 tangent{};
					tangent.x = file.Read<float>();
					tangent.y = file.Read<float>();
					tangent.z = file.Read<float>();
					mTangents.push_back(tangent);
				}
				break;
			case CoordSystem::RightHanded:
				for (size_t i{ 0 }; i < vertexCount; ++i)
				{
					DirectX::XMFLOAT3 tangent{};
					tangent.x = file.Read<float>();
					tangent.z = file.Read<float>();
					tangent.y = -file.Read<float>();
					mTangents.push_back(tangent);
				}
				break;
			}
			break;
		case MeshType::Binormals:
			mElem[size_t(InputSemantic::Binormal)] = 1;
			switch (mCoordSystem)
			{
			case CoordSystem::LeftHanded:
				for (size_t i{ 0 }; i < vertexCount; ++i)
				{
					DirectX::XMFLOAT3 binormal{};
					binormal.x = file.Read<float>();
					binormal.y = file.Read<float>();
					binormal.z = file.Read<float>();
					mBinormals.push_back(binormal);
				}
				break;
			case CoordSystem::RightHanded:
				for (size_t i{ 0 }; i < vertexCount; ++i)
				{
					DirectX::XMFLOAT3 binormal{};
					binormal.x = file.Read<float>();
					binormal.z = file.Read<float>();
					binormal.y = -file.Read<float>();
					mBinormals.push_back(binormal);
				}
				break;
			}
			break;
		case MeshType::TexCoords:
			mElem[size_t(InputSemantic::TexCoord)] = 1;
			mTexCoordCount = file.Read<USHORT>();
			for (size_t i{ 0 }; i < vertexCount * mTexCoordCount; ++i)
			{
				DirectX::XMFLOAT2 texCoord{};
				texCoord.x = file.Read<float>();
				texCoord.y = file.Read<float>();
				mTexCoords.push_back(texCoord);
			}
			break;
		case MeshType::Colors:
			mElem[size_t(InputSemantic::Color)] = 1;
			for (size_t i{ 0 }; i < vertexCount; ++i)
			{
				DirectX::XMFLOAT4 color{};
				color.x = file.Read<float>();
				color.y = file.Read<float>();
				color.z = file.Read<float>();
				color.w = file.Read<float>();
				mColors.push_back(color);
			}
			break;
		case MeshType::BlendIndices:
			mElem[size_t(InputSemantic::BlendIndices)] = 1;
			for (size_t i{ 0 }; i < vertexCount; ++i)
			{
				Int4 index{};
				index.mA = int(file.Read<float>());
				index.mB = int(file.Read<float>());
				index.mC = int(file.Read<float>());
				index.mD = int(file.Read<float>());
				mBlendIndices.push_back(index);
			}
			break;
		case MeshType::BlendWeights:
			mElem[size_t(InputSemantic::BlendWeight)] = 1;
			for (size_t i{ 0 }; i < vertexCount; ++i)
			{
				DirectX::XMFLOAT4 weight{};
				weight.x = file.Read<float>();
				weight.y = file.Read<float>();
				weight.z = file.Read<float>();
				weight.w = file.Read<float>();
				mBlendWeights.push_back(weight);
			}
			break;
		case MeshType::AnimationClips:
		{
			mHasAnimation = true;
			DirectX::XMFLOAT4X4 toggleHand{
				1.f, 0.f, 0.f, 0.f,
				0.f, 0.f, -1.f, 0.f,
				0.f, 1.f, 0.f, 0.f,
				0.f, 0.f, 0.f, 1.f};
			DirectX::XMMATRIX matrixToggle{ DirectX::XMLoadFloat4x4(&toggleHand) };
			unsigned short clipCount{ file.Read<unsigned short>() };
			for (unsigned short i{ 0 }; i < clipCount; ++i)
			{
				AnimationClip clip{};
				clip.SetName(file.ReadString());
				clip.SetDuration(file.Read<float>());
				clip.SetTicksPerSecond(file.Read<float>());
				unsigned short keyCount{ file.Read<unsigned short>() };
				for (unsigned short j{ 0 }; j < keyCount; ++j)
				{
					AnimationKey key{};
					key.SetTick(file.Read<float>());
					unsigned short transformCount{ file.Read<unsigned short>() };
					for (unsigned short k{ 0 }; k < transformCount; ++k)
					{
						DirectX::XMFLOAT4X4 transform{};
						//for (int l{ 0 }; l < 16; ++l)
						//	transform(l / 4, l % 4) = file.Read<float>();
						transform(0, 0) = file.Read<float>();
						transform(0, 1) = file.Read<float>();
						transform(0, 2) = file.Read<float>();
						transform(0, 3) = file.Read<float>();
						transform(1, 0) = file.Read<float>();
						transform(1, 1) = file.Read<float>();
						transform(1, 2) = file.Read<float>();
						transform(1, 3) = file.Read<float>();
						transform(2, 0) = file.Read<float>();
						transform(2, 1) = file.Read<float>();
						transform(2, 2) = file.Read<float>();
						transform(2, 3) = file.Read<float>();
						transform(3, 0) = file.Read<float>();
						transform(3, 1) = file.Read<float>();
						transform(3, 2) = file.Read<float>();
						transform(3, 3) = file.Read<float>();
						if (mCoordSystem == CoordSystem::RightHanded)
						{
							DirectX::XMMATRIX matrixTransform{ DirectX::XMLoadFloat4x4(&transform) };
							DirectX::XMStoreFloat4x4(&transform, matrixToggle * matrixTransform);
						}
						key.GetBoneTransforms().push_back(transform);
					}
					clip.GetKeys().emplace_back(key);
				}
				mAnimationClips.emplace_back(clip);
			}
		}
		break;
		case MeshType::Skeleton:
		{
			mBoneCount = size_t(file.Read<unsigned short>());
			file.MovePosition(offset - 2);
		}
		break;
		default:
			file.MovePosition(offset);
			break;

		}
	}
}
