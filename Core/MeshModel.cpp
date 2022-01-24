#include "pch.h"
#include "MeshModel.h"
#include "Application.h"
#include "MeshType.h"
#include "Material3D.h"

using namespace Ion;

Core::MeshModel::MeshModel(Core::Application* pApplication, const std::string& fileName, const std::string& fileExtension, Core::Winding winding, Core::CoordSystem coordSystem)
	: mIsInitialized{ false }
	, mpApplication{ pApplication }
	, mFileName{ fileName }
	, mFileExtension{ fileExtension }
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

void Core::MeshModel::Initialize()
{
	if (mIsInitialized)
		return;
	mIsInitialized = true;
}

const std::vector<DWORD>& Core::MeshModel::GetIndices() const
{
	return mIndices;
}

const std::vector<DirectX::XMFLOAT3>& Core::MeshModel::GetPositions() const
{
	return mPositions;
}

const std::vector<DirectX::XMFLOAT3>& Core::MeshModel::GetNormals() const
{
	return mNormals;
}

const std::vector<DirectX::XMFLOAT3>& Core::MeshModel::GetTangents() const
{
	return mTangents;
}

const std::vector<DirectX::XMFLOAT3>& Core::MeshModel::GetBinormals() const
{
	return mBinormals;
}

const std::vector<DirectX::XMFLOAT2>& Core::MeshModel::GetTexCoords() const
{
	return mTexCoords;
}

const std::vector<DirectX::XMFLOAT4>& Core::MeshModel::GetColors() const
{
	return mColors;
}

const std::vector<Core::Int4>& Core::MeshModel::GetBlendIndices() const
{
	return mBlendIndices;
}

const std::vector<DirectX::XMFLOAT4>& Core::MeshModel::GetBlendWeights() const
{
	return mBlendWeights;
}

const std::vector<Core::AnimationClip>& Core::MeshModel::GetAnimationClips() const
{
	return mAnimationClips;
}

std::size_t Core::MeshModel::GetElementCount()
{
	std::size_t cnt{ 0 };
	for (std::size_t i{ 0 }; i < std::size_t(Core::InputSemantic::Count); ++i)
		cnt += mElem[i];
	return cnt;
}

bool Core::MeshModel::HasInputElem(Core::InputSemantic inputSemantic)
{
	return (mElem[std::size_t(inputSemantic)] == 1);
}

bool Core::MeshModel::HasInputElem(const std::string& inputSemantic)
{
	return HasInputElem(Core::Material3D::GetSemanticStrings().at(inputSemantic).inputSemantic);
}

const std::vector<Core::Transform>& Core::MeshModel::ReadInstances()
{
	if (mInstances.empty())
	{
		Core::BinIfstream file{ mpApplication, "../Resources/Instance/" + mFileName + ".ins" };
		std::size_t cnt{ file.Read<std::size_t>() };
		for (std::size_t i{ 0 }; i < cnt; ++i)
			mInstances.emplace_back(file.Read<Core::Transform>());
	}
	return mInstances;
}

const std::vector<Core::Transform>& Core::MeshModel::GetInstances() const
{
	return mInstances;
}

void Core::MeshModel::ReadModel()
{
	using namespace DirectX;
	Core::BinIfstream file{ mpApplication, "../Resources/Model/" + mFileName + "." + mFileExtension };
	const char majorVersion{ file.Read<char>() };
	(majorVersion);
	const char minorVersion{ file.Read<char>() };
	(minorVersion);

	std::size_t
		indexCount{ 0 },
		vertexCount{ 0 };
	for (;;)
	{
		Core::MeshType meshType{ Core::MeshType(file.Read<char>()) };
		if (meshType == Core::MeshType::End)
			break;
		unsigned int offset{ file.Read<unsigned int>() };

		switch (meshType)
		{
		case Core::MeshType::Header:
			mName = file.ReadString();
			vertexCount = std::size_t(file.Read<unsigned int>());
			indexCount = std::size_t(file.Read<unsigned int>());
			break;
		case Core::MeshType::Positions:
			mElem[std::size_t(Core::InputSemantic::Position)] = 1;
			switch (mCoordSystem)
			{
			case Core::CoordSystem::LeftHanded:
				for (std::size_t i{ 0 }; i < vertexCount; ++i)
				{
					DirectX::XMFLOAT3 position{};
					position.x = file.Read<float>();
					position.y = file.Read<float>();
					position.z = file.Read<float>();
					mPositions.push_back(position);
				}
				break;
			case Core::CoordSystem::RightHanded:
				for (std::size_t i{ 0 }; i < vertexCount; ++i)
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
		case Core::MeshType::Indices:
		{
			std::size_t cnt{ indexCount / 3 };
			switch (mWinding)
			{
			case Core::Winding::CW:
				for (std::size_t i{ 0 }; i < cnt; ++i)
				{
					mIndices.push_back(file.Read<DWORD>());
					mIndices.push_back(file.Read<DWORD>());
					mIndices.push_back(file.Read<DWORD>());
				}
				break;
			case Core::Winding::CCW:
				DWORD two{};
				for (std::size_t i{ 0 }; i < cnt; ++i)
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
		case Core::MeshType::Normals:
			mElem[std::size_t(Core::InputSemantic::Normal)] = 1;
			switch (mCoordSystem)
			{
			case Core::CoordSystem::LeftHanded:
				for (std::size_t i{ 0 }; i < vertexCount; ++i)
				{
					DirectX::XMFLOAT3 normal{};
					normal.x = file.Read<float>();
					normal.y = file.Read<float>();
					normal.z = file.Read<float>();
					mNormals.push_back(normal);
				}
				break;
			case Core::CoordSystem::RightHanded:
				for (std::size_t i{ 0 }; i < vertexCount; ++i)
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
		case Core::MeshType::Tangents:
			mElem[std::size_t(Core::InputSemantic::Tangent)] = 1;
			switch (mCoordSystem)
			{
			case Core::CoordSystem::LeftHanded:
				for (std::size_t i{ 0 }; i < vertexCount; ++i)
				{
					DirectX::XMFLOAT3 tangent{};
					tangent.x = file.Read<float>();
					tangent.y = file.Read<float>();
					tangent.z = file.Read<float>();
					mTangents.push_back(tangent);
				}
				break;
			case Core::CoordSystem::RightHanded:
				for (std::size_t i{ 0 }; i < vertexCount; ++i)
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
		case Core::MeshType::Binormals:
			mElem[std::size_t(Core::InputSemantic::Binormal)] = 1;
			switch (mCoordSystem)
			{
			case Core::CoordSystem::LeftHanded:
				for (std::size_t i{ 0 }; i < vertexCount; ++i)
				{
					DirectX::XMFLOAT3 binormal{};
					binormal.x = file.Read<float>();
					binormal.y = file.Read<float>();
					binormal.z = file.Read<float>();
					mBinormals.push_back(binormal);
				}
				break;
			case Core::CoordSystem::RightHanded:
				for (std::size_t i{ 0 }; i < vertexCount; ++i)
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
		case Core::MeshType::TexCoords:
			mElem[std::size_t(Core::InputSemantic::TexCoord)] = 1;
			mTexCoordCount = file.Read<USHORT>();
			for (std::size_t i{ 0 }; i < vertexCount * mTexCoordCount; ++i)
			{
				DirectX::XMFLOAT2 texCoord{};
				texCoord.x = file.Read<float>();
				texCoord.y = file.Read<float>();
				mTexCoords.push_back(texCoord);
			}
			break;
		case Core::MeshType::Colors:
			mElem[std::size_t(Core::InputSemantic::Color)] = 1;
			for (std::size_t i{ 0 }; i < vertexCount; ++i)
			{
				DirectX::XMFLOAT4 color{};
				color.x = file.Read<float>();
				color.y = file.Read<float>();
				color.z = file.Read<float>();
				color.w = file.Read<float>();
				mColors.push_back(color);
			}
			break;
		case Core::MeshType::BlendIndices:
			mElem[std::size_t(Core::InputSemantic::BlendIndices)] = 1;
			for (std::size_t i{ 0 }; i < vertexCount; ++i)
			{
				Int4 index{};
				index.mA = int(file.Read<float>());
				index.mB = int(file.Read<float>());
				index.mC = int(file.Read<float>());
				index.mD = int(file.Read<float>());
				mBlendIndices.push_back(index);
			}
			break;
		case Core::MeshType::BlendWeights:
			mElem[std::size_t(Core::InputSemantic::BlendWeight)] = 1;
			for (std::size_t i{ 0 }; i < vertexCount; ++i)
			{
				DirectX::XMFLOAT4 weight{};
				weight.x = file.Read<float>();
				weight.y = file.Read<float>();
				weight.z = file.Read<float>();
				weight.w = file.Read<float>();
				mBlendWeights.push_back(weight);
			}
			break;
		case Core::MeshType::AnimationClips:
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
				Core::AnimationClip clip{};
				clip.SetName(file.ReadString());
				clip.SetDuration(file.Read<float>());
				clip.SetTicksPerSecond(file.Read<float>());
				unsigned short keyCount{ file.Read<unsigned short>() };
				for (unsigned short j{ 0 }; j < keyCount; ++j)
				{
					Core::AnimationKey key{};
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
						if (mCoordSystem == Core::CoordSystem::RightHanded)
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
		case Core::MeshType::Skeleton:
		{
			mBoneCount = std::size_t(file.Read<unsigned short>());
			file.MovePosition(std::size_t(offset) - sizeof(unsigned short));
		}
		break;
		default:
			file.MovePosition(offset);
			break;

		}
	}
}
