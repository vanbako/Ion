#include "../Core/pch.h"
#include "../Core/Material.h"
#include "../Core/Application.h"
#include "../Core/Canvas.h"
#include "../Core/d3dx12.h"
#include <d3dcompiler.h>

using namespace Ion::Core;

const UINT Material::mMaxInputParam{ 20 };
const std::map<std::string, SemanticInfo> Material::mSemanticStrings
{
	{ "POSITION", SemanticInfo{ InputSemantic::Position, DXGI_FORMAT_R32G32B32_FLOAT, 12 } },
	{ "POSITION0", SemanticInfo{ InputSemantic::Position0, DXGI_FORMAT_R32G32B32_FLOAT, 12 } },
	{ "POSITION1", SemanticInfo{ InputSemantic::Position1, DXGI_FORMAT_R32G32B32_FLOAT, 12 } },
	{ "POSITION2", SemanticInfo{ InputSemantic::Position2, DXGI_FORMAT_R32G32B32_FLOAT, 12 } },
	{ "NORMAL", SemanticInfo{ InputSemantic::Normal, DXGI_FORMAT_R32G32B32_FLOAT, 12 } },
	{ "NORMAL0", SemanticInfo{ InputSemantic::Normal0, DXGI_FORMAT_R32G32B32_FLOAT, 12 } },
	{ "NORMAL1", SemanticInfo{ InputSemantic::Normal1, DXGI_FORMAT_R32G32B32_FLOAT, 12 } },
	{ "NORMAL2", SemanticInfo{ InputSemantic::Normal2, DXGI_FORMAT_R32G32B32_FLOAT, 12 } },
	{ "TANGENT", SemanticInfo{ InputSemantic::Tangent, DXGI_FORMAT_R32G32B32_FLOAT, 12 } },
	{ "TANGENT0", SemanticInfo{ InputSemantic::Tangent0, DXGI_FORMAT_R32G32B32_FLOAT, 12 } },
	{ "TANGENT1", SemanticInfo{ InputSemantic::Tangent1, DXGI_FORMAT_R32G32B32_FLOAT, 12 } },
	{ "TANGENT2", SemanticInfo{ InputSemantic::Tangent2, DXGI_FORMAT_R32G32B32_FLOAT, 12 } },
	{ "BINORMAL", SemanticInfo{ InputSemantic::Binormal, DXGI_FORMAT_R32G32B32_FLOAT, 12 } },
	{ "BINORMAL0", SemanticInfo{ InputSemantic::Binormal0, DXGI_FORMAT_R32G32B32_FLOAT, 12 } },
	{ "BINORMAL1", SemanticInfo{ InputSemantic::Binormal1, DXGI_FORMAT_R32G32B32_FLOAT, 12 } },
	{ "BINORMAL2", SemanticInfo{ InputSemantic::Binormal2, DXGI_FORMAT_R32G32B32_FLOAT, 12 } },
	{ "TEXCOORD", SemanticInfo{ InputSemantic::TexCoord, DXGI_FORMAT_R32G32_FLOAT, 8 } },
	{ "TEXCOORD0", SemanticInfo{ InputSemantic::TexCoord0, DXGI_FORMAT_R32G32_FLOAT, 8 } },
	{ "TEXCOORD1", SemanticInfo{ InputSemantic::TexCoord1, DXGI_FORMAT_R32G32_FLOAT, 8 } },
	{ "TEXCOORD2", SemanticInfo{ InputSemantic::TexCoord2, DXGI_FORMAT_R32G32_FLOAT, 8 } },
	{ "COLOR", SemanticInfo{ InputSemantic::Color, DXGI_FORMAT_R32G32B32A32_FLOAT, 16 } },
	{ "COLOR0", SemanticInfo{ InputSemantic::Color0, DXGI_FORMAT_R32G32B32A32_FLOAT, 16 } },
	{ "COLOR1", SemanticInfo{ InputSemantic::Color1, DXGI_FORMAT_R32G32B32A32_FLOAT, 16 } },
	{ "COLOR2", SemanticInfo{ InputSemantic::Color2, DXGI_FORMAT_R32G32B32A32_FLOAT, 16 } },
	{ "BLENDINDICES", SemanticInfo{ InputSemantic::BlendIndices, DXGI_FORMAT_R32G32B32A32_FLOAT, 16 } },
	{ "BLENDINDICES0", SemanticInfo{ InputSemantic::BlendIndices0, DXGI_FORMAT_R32G32B32A32_FLOAT, 16 } },
	{ "BLENDINDICES1", SemanticInfo{ InputSemantic::BlendIndices1, DXGI_FORMAT_R32G32B32A32_FLOAT, 16 } },
	{ "BLENDINDICES2", SemanticInfo{ InputSemantic::BlendIndices2, DXGI_FORMAT_R32G32B32A32_FLOAT, 16 } },
	{ "BLENDWEIGHT", SemanticInfo{ InputSemantic::BlendWeight, DXGI_FORMAT_R32G32B32A32_FLOAT, 16 } },
	{ "BLENDWEIGHT0", SemanticInfo{ InputSemantic::BlendWeight0, DXGI_FORMAT_R32G32B32A32_FLOAT, 16 } },
	{ "BLENDWEIGHT1", SemanticInfo{ InputSemantic::BlendWeight1, DXGI_FORMAT_R32G32B32A32_FLOAT, 16 } },
	{ "BLENDWEIGHT2", SemanticInfo{ InputSemantic::BlendWeight2, DXGI_FORMAT_R32G32B32A32_FLOAT, 16 } }
};
const std::map<std::string, TextureType> Material::mTextureTypeStrings
{
	{ "gTextureAlbedo", TextureType::Albedo },
	{ "gTextureNormal", TextureType::Normal },
	{ "gTextureRoughness", TextureType::Roughness },
	{ "gTextureMetalness", TextureType::Metalness },
	{ "gTextureSpecular", TextureType::Specular },
	{ "gTextureHeight", TextureType::Height },
	{ "gTextureOpacity", TextureType::Opacity },
	{ "gTextureAmbientOcclusion", TextureType::AmbientOcclusion },
	{ "gTextureRefraction", TextureType::Refraction },
	{ "gTextureSelfIllumination", TextureType::SelfIllumination }
};

Material::Material(Application* pApplication, const std::string& name)
	: mIsInitialized{ false }
	, mpApplication{ pApplication }
	, mName{ name }
	, mRS{}
	, mVS{}
	, mPS{}
	, mpRootSignature{}
	, mpPipelineState{}
	, mSigParDescs{}
	, mpInputElementDescs{ nullptr }
	, mConstantBufferCount{ 0 }
	, mInputElementCount{ 0 }
	, mLayoutSize{ 0 }
	, mpCanvasViewCs{}
{
	auto pDevice{ mpApplication->GetDevice() };
#ifdef _DEBUG
	std::string dir{ "Debug" };
#else
	std::string dir{ "Release" };
#endif

	// Build Root Signature
	{
		std::string str{ "../x64/" + dir + "/" + mName + "_rs.cso" };
		std::wstring wstr(str.begin(), str.end());
		ThrowIfFailed(D3DReadFileToBlob(wstr.c_str(), &mRS));
		ThrowIfFailed(pDevice->CreateRootSignature(0, mRS->GetBufferPointer(), mRS->GetBufferSize(), IID_PPV_ARGS(&mpRootSignature)));
	}

	// Shaders
	{
		std::string str{ "../x64/" + dir + "/" + mName + "_vs.cso" };
		std::wstring wstr(str.begin(), str.end());
		ThrowIfFailed(D3DReadFileToBlob(wstr.c_str(), &mVS));
	}
	{
		std::string str{ "../x64/" + dir + "/" + mName + "_ps.cso" };
		std::wstring wstr(str.begin(), str.end());
		ThrowIfFailed(D3DReadFileToBlob(wstr.c_str(), &mPS));
	}

	ID3D12ShaderReflection* pVSReflector{ nullptr };
	ThrowIfFailed(D3DReflect(mVS->GetBufferPointer(), mVS->GetBufferSize(), IID_PPV_ARGS(&pVSReflector)));
	// Build texture types set supported by shader
	ID3D12ShaderReflection* pPSReflector{ nullptr };
	ThrowIfFailed(D3DReflect(mPS->GetBufferPointer(), mPS->GetBufferSize(), IID_PPV_ARGS(&pPSReflector)));
	D3D12_SHADER_INPUT_BIND_DESC shaderBindDesc{};
	for (auto& pair : mTextureTypeStrings)
		if (pPSReflector->GetResourceBindingDescByName(pair.first.c_str(), &shaderBindDesc) == S_OK)
			mTextureTypes.emplace(pair.second);
	// Read & build Input layout from Vertex shader
	while (pVSReflector->GetInputParameterDesc(mInputElementCount, &mSigParDescs[mInputElementCount]) == S_OK)
		++mInputElementCount;
	mpInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[mInputElementCount];
	for (UINT i{ 0 }; i < mInputElementCount; ++i)
	{
		const SemanticInfo& semanticInfo{ mSemanticStrings.at(mSigParDescs[i].SemanticName) };
		mpInputElementDescs[i] = {
			mSigParDescs[i].SemanticName,
			mSigParDescs[i].SemanticIndex,
			semanticInfo.format,
			0,
			mLayoutSize,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		mLayoutSize += semanticInfo.offset;
	}
}

Material::~Material()
{
	if (mpInputElementDescs != nullptr)
		delete[] mpInputElementDescs;
}

void Material::Initialize()
{
	if (mIsInitialized)
		return;
	if (mpInputElementDescs == nullptr)
		return;
	auto pDxgiFactory{ mpApplication->GetDxgiFactory() };
	auto pDevice{ mpApplication->GetDevice() };
	auto pCommandAllocator{ mpApplication->GetCommandAllocator() };

	// Build Pipeline State
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc{};
		psoDesc.InputLayout = { mpInputElementDescs, mInputElementCount };
		psoDesc.pRootSignature = mpRootSignature.Get();
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(mVS.Get());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(mPS.Get());
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		psoDesc.SampleDesc.Count = 1;
		ThrowIfFailed(pDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mpPipelineState)));
	}
	mIsInitialized = true;
}

void Material::Render(Canvas* pCanvas)
{
	auto pCommandAllocator{ mpApplication->GetCommandAllocator() };
	auto pGraphicsCommandList{ pCanvas->GetGraphicsCommandList() };

	pGraphicsCommandList->SetPipelineState(mpPipelineState.Get());
	pGraphicsCommandList->SetGraphicsRootSignature(mpRootSignature.Get());

	pCanvas->SetDescriptor();

	for (auto pViewC : mpCanvasViewCs[pCanvas])
		pViewC->Render(pCanvas, this);
}

const Microsoft::WRL::ComPtr<ID3D12RootSignature>& Material::GetRootSignature()
{
	return mpRootSignature;
}

const Microsoft::WRL::ComPtr<ID3D12PipelineState>& Material::GetPipelineState()
{
	return mpPipelineState;
}

D3D12_INPUT_ELEMENT_DESC* Material::GetInputElementDescs()
{
	return mpInputElementDescs;
}

UINT Material::GetInputElementCount() const
{
	return mInputElementCount;
}

UINT Material::GetLayoutSize() const
{
	return mLayoutSize;
}

const std::set<TextureType>& Material::GetTextureTypeSet() const
{
	return mTextureTypes;
}

void Material::AddViewC(Canvas* pCanvas, ViewC* pViewC)
{
	mpCanvasViewCs[pCanvas].emplace_back(pViewC);
}

const std::map<std::string, SemanticInfo>& Material::GetSemanticStrings()
{
	return mSemanticStrings;
}
