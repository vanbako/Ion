#include "pch.h"
#include "Material3D.h"
#include "Application.h"
#include "Canvas.h"
#include <d3dcompiler.h>
#include <atlfile.h>

using namespace Ion;

Core::Vector<long long> Core::Material3D::mViewCCubeSize{ 100, 100, 100 };
const UINT Core::Material3D::mMaxInputParam{ 20 };
const std::unordered_map<std::string, Core::SemanticInfo> Core::Material3D::mSemanticStrings
{
	{ "POSITION", SemanticInfo{ InputSemantic::Position, DXGI_FORMAT_R32G32B32_FLOAT, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA } },
	{ "NORMAL", SemanticInfo{ InputSemantic::Normal, DXGI_FORMAT_R32G32B32_FLOAT, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA } },
	{ "TANGENT", SemanticInfo{ InputSemantic::Tangent, DXGI_FORMAT_R32G32B32_FLOAT, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA } },
	{ "BINORMAL", SemanticInfo{ InputSemantic::Binormal, DXGI_FORMAT_R32G32B32_FLOAT, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA } },
	{ "TEXCOORD", SemanticInfo{ InputSemantic::TexCoord, DXGI_FORMAT_R32G32_FLOAT, 8, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA } },
	{ "COLOR", SemanticInfo{ InputSemantic::Color, DXGI_FORMAT_R32G32B32A32_FLOAT, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA } },
	{ "BLENDINDICES", SemanticInfo{ InputSemantic::BlendIndices, DXGI_FORMAT_R32G32B32A32_SINT, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA } },
	{ "BLENDWEIGHT", SemanticInfo{ InputSemantic::BlendWeight, DXGI_FORMAT_R32G32B32A32_FLOAT, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA } },
	{ "WORLD", SemanticInfo{ InputSemantic::World, DXGI_FORMAT_R32G32B32A32_FLOAT, 16, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA } }
};
const std::unordered_map<std::string, Core::TextureType> Core::Material3D::mTextureTypeStrings
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

Core::Material3D::Material3D(Core::Application* pApplication, const std::string& name)
	: Core::Material(pApplication, name)
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
	, mpCanvasCubes{}
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
		mpApplication->ThrowIfFailed(D3DReadFileToBlob(wstr.c_str(), &mRS));
		mpApplication->ThrowIfFailed(pDevice->CreateRootSignature(0, mRS->GetBufferPointer(), mRS->GetBufferSize(), IID_PPV_ARGS(&mpRootSignature)));
	}

	// Shaders
	{
		std::string str{ "../x64/" + dir + "/" + mName + "_vs.cso" };
		std::wstring wstr(str.begin(), str.end());
		mpApplication->ThrowIfFailed(D3DReadFileToBlob(wstr.c_str(), &mVS));
	}
	{
		std::string str{ "../x64/" + dir + "/" + mName + "_ps.cso" };
		std::wstring wstr(str.begin(), str.end());
		mpApplication->ThrowIfFailed(D3DReadFileToBlob(wstr.c_str(), &mPS));
	}

        HMODULE dxcDll{ LoadLibraryW(L"dxcompiler.dll") };
        if (!dxcDll)
                mpApplication->ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
        auto dxcCreateInstanceProc{ (DxcCreateInstanceProc)GetProcAddress(dxcDll, "DxcCreateInstance") };
        if (!dxcCreateInstanceProc)
        {
                FreeLibrary(dxcDll);
                mpApplication->ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
        }

        {
                ATL::CComPtr<IDxcContainerReflection> pContainerReflection{};
                mpApplication->ThrowIfFailed(dxcCreateInstanceProc(CLSID_DxcContainerReflection, IID_PPV_ARGS(&pContainerReflection)));

        ATL::CComPtr<ID3D12ShaderReflection> pVSReflector{};
        ATL::CComPtr<IDxcBlob> pVS{};
        mVS->QueryInterface(&pVS);
        pContainerReflection->Load(pVS);
        UINT32 shaderIdx{};
        pContainerReflection->FindFirstPartKind(DXC_PART_DXIL, &shaderIdx);
        pContainerReflection->GetPartReflection(shaderIdx, IID_PPV_ARGS(&pVSReflector));

	// Build texture types set supported by shader
        ATL::CComPtr<ID3D12ShaderReflection> pPSReflector{};
        ATL::CComPtr<IDxcBlob> pPS{};
	mPS->QueryInterface(&pPS);
	pContainerReflection->Load(pPS);
	D3D12_SHADER_INPUT_BIND_DESC shaderBindDesc{};
	pContainerReflection->FindFirstPartKind(DXC_PART_DXIL, &shaderIdx);
	pContainerReflection->GetPartReflection(shaderIdx, IID_PPV_ARGS(&pPSReflector));
	for (auto& pair : mTextureTypeStrings)
	{
		if (pPSReflector->GetResourceBindingDescByName(pair.first.c_str(), &shaderBindDesc) == S_OK)
			mTextureTypes.emplace(pair.second);
	}
	// Read & build Input layout from Vertex shader
	while (pVSReflector->GetInputParameterDesc(mInputElementCount, &mSigParDescs[mInputElementCount]) == S_OK)
		++mInputElementCount;
	mpInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[mInputElementCount];
	UINT svInstanceID{ 0 };
        for (UINT i{ 0 }; i < mInputElementCount; ++i)
        {
                // SV_InstanceID is generated by DirectX and should be last
                if (std::strcmp(mSigParDescs[i].SemanticName, "SV_INSTANCEID") == 0)
                {
			svInstanceID = 1;
			continue;
		}
		const Core::SemanticInfo& semanticInfo{ mSemanticStrings.at(mSigParDescs[i].SemanticName) };
		mpInputElementDescs[i] = {
			mSigParDescs[i].SemanticName,
			mSigParDescs[i].SemanticIndex,
			semanticInfo.format,
			0,
			mLayoutSize,
			semanticInfo.inputClassification,
			0 };
                mLayoutSize += semanticInfo.offset;
        }
        mInputElementCount -= svInstanceID;
        }
        if (dxcDll)
                FreeLibrary(dxcDll);
}

Core::Material3D::~Material3D()
{
	if (mpInputElementDescs != nullptr)
		delete[] mpInputElementDescs;
}

void Core::Material3D::Initialize()
{
	// TODO: This should not happen, so only in debug and throw error
	if (mIsInitialized)
		return;
	Core::Material::Initialize();
	if (mpInputElementDescs == nullptr)
		return;
	auto pDxgiFactory{ mpApplication->GetDxgiFactory() };
	auto pDevice{ mpApplication->GetDevice() };

	// Build Pipeline State
	{
		D3D12_RASTERIZER_DESC rasDesc{};
		rasDesc.FillMode = D3D12_FILL_MODE_SOLID;
		rasDesc.CullMode = D3D12_CULL_MODE_BACK;
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc{};
		psoDesc.InputLayout = { mpInputElementDescs, mInputElementCount };
		psoDesc.pRootSignature = mpRootSignature.Get();
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(mVS.Get());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(mPS.Get());
		psoDesc.RasterizerState = rasDesc;
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		psoDesc.SampleDesc.Count = 1;
		mpApplication->ThrowIfFailed(pDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mpPipelineState)));
	}
	mIsInitialized = true;
}

void Core::Material3D::Render(Core::Canvas* pCanvas)
{
	if (!mIsInitialized)
	{
#ifdef ION_LOGGER
		mpApplication->GetServiceLocator().GetLogger()->Message(typeid(this).name(), Core::MsgType::Fatal, "Material3D.Render() while mIsInitialized == false");
#endif
		return;
	}
	auto pGraphicsCommandList{ pCanvas->GetGraphicsCommandList() };

	pGraphicsCommandList->SetPipelineState(mpPipelineState.Get());
	pGraphicsCommandList->SetGraphicsRootSignature(mpRootSignature.Get());

	pCanvas->SetDescriptor();

	std::multimap<long long, Core::ViewCCube>& cubes{ mpCanvasCubes[pCanvas] };
	for (auto& cube : cubes)
		cube.second.Render(pCanvas, this);
}

void Core::Material3D::ViewCUpdate(Core::Canvas* pCanvas, float delta)
{
	std::multimap<long long, Core::ViewCCube>& cubes{ mpCanvasCubes[pCanvas] };
	for (auto& cube : cubes)
		cube.second.ViewCUpdate(pCanvas, delta);
}

const Microsoft::WRL::ComPtr<ID3D12RootSignature>& Core::Material3D::GetRootSignature()
{
	return mpRootSignature;
}

const Microsoft::WRL::ComPtr<ID3D12PipelineState>& Core::Material3D::GetPipelineState()
{
	return mpPipelineState;
}

D3D12_INPUT_ELEMENT_DESC* Core::Material3D::GetInputElementDescs()
{
	return mpInputElementDescs;
}

UINT Core::Material3D::GetInputElementCount() const
{
	return mInputElementCount;
}

UINT Core::Material3D::GetLayoutSize() const
{
	return mLayoutSize;
}

const std::set<Core::TextureType>& Core::Material3D::GetTextureTypeSet() const
{
	return mTextureTypes;
}

void Core::Material3D::AddViewC(Core::Canvas* pCanvas, Core::ViewC* pViewC)
{
	std::multimap<long long, Core::ViewCCube>& cubes{ mpCanvasCubes[pCanvas] };
	Core::Vector<long long> cubePos{ GetViewCCubePos(pViewC) };
	auto itCube{ GetViewCCubeIterator(cubePos, cubes) };
	(*itCube).second.AddViewC(false, pViewC);
	pViewC->SetCube(&(*itCube).second);
}

const std::unordered_map<std::string, Core::SemanticInfo>& Core::Material3D::GetSemanticStrings()
{
	return mSemanticStrings;
}

// If new pos is in the same cube as current pos
//   return
// hasmoved = true in current
// if exists in new cube
//   hasmoved = false in new
// else
//   add in new with hasMoved = false

void Core::Material3D::MoveViewC(Core::Canvas* pCanvas, Core::ViewC* pViewC, Core::ViewCCube* pCurrCube)
{
	Core::Vector<long long> cubePos{ GetViewCCubePos(pViewC) };
	if (pCurrCube->GetLocation() == cubePos)
		return;
	pCurrCube->SetHasMoved(pViewC, true);
	std::multimap<long long, Core::ViewCCube>& cubes{ mpCanvasCubes[pCanvas] };
	auto itCube{ GetViewCCubeIterator(cubePos, cubes) };
	(*itCube).second.AddViewCCheckExistence(false, pViewC);
	pViewC->SetCube(&(*itCube).second);
}

void Core::Material3D::AddViewCToCube(std::multimap<long long, Core::ViewCCube>& cubes, Core::ViewC* pViewC)
{
	Core::Vector<long long> cubePos{ GetViewCCubePos(pViewC) };
	auto itCube{ GetViewCCubeIterator(cubePos, cubes) };
	(*itCube).second.AddViewC(false, pViewC);
	pViewC->SetCube(&(*itCube).second);
}

Core::Vector<long long> Core::Material3D::GetViewCCubePos(Core::ViewC* pViewC)
{
	DirectX::XMFLOAT4 pos{ pViewC->GetObject()->GetModelC<TransformMC>()->GetWorldPosition() };
	return Core::Vector<long long>{
		long long(pos.x / mViewCCubeSize.GetA()),
		long long(pos.y / mViewCCubeSize.GetB()),
		long long(pos.z / mViewCCubeSize.GetC()) };
}

std::multimap<long long, Core::ViewCCube>::iterator Core::Material3D::GetViewCCubeIterator(Core::Vector<long long>& cubePos, std::multimap<long long, Core::ViewCCube>& cubes)
{
	long long key{ cubePos.GetA() * cubePos.GetA() + cubePos.GetB() * cubePos.GetB() + cubePos.GetC() * cubePos.GetC() };
	auto itCube{ cubes.end() };
	for (auto it{ cubes.lower_bound(key) }; it != cubes.upper_bound(key); ++it)
		if ((*it).second.GetLocation() == cubePos)
		{
			itCube = it;
			break;
		}
	if (itCube == cubes.end())
	{
		itCube = cubes.emplace(key, cubePos);
		(*itCube).second.SetMaterial3D(this);
	}
	return itCube;
}
