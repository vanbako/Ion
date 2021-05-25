#include "../Core/pch.h"
#include "../Core/Material.h"
#include "../Core/Application.h"
#include "../Core/Canvas.h"
#include "../Core/d3dx12.h"
#include <d3dcompiler.h>

using namespace Ion::Core;

Material::Material(Application* pApplication, const std::string& name)
	: mIsInitialized{ false }
	, mpApplication{ pApplication }
	, mName{ name }
	, mVS{}
	, mPS{}
	, mpRootSignature{}
	, mpPipelineState{}
{
}

Material::~Material()
{
}

void Material::Initialize()
{
	if (mIsInitialized)
		return;
	auto pDxgiFactory{ mpApplication->GetDxgiFactory() };
	auto pDevice{ mpApplication->GetDevice() };
	auto pCommandAllocator{ mpApplication->GetCommandAllocator() };

#ifdef _DEBUG
	std::string dir{ "Debug" };
#else
	std::string dir{ "Release" };
#endif
	std::string name{ "PosNormCol" };

	// Build Root Signature
	{
		Microsoft::WRL::ComPtr<ID3DBlob> signature{};
		std::string str{ "../x64/" + dir + "/" + name + "_rs.cso" };
		std::wstring wstr(str.begin(), str.end());
		ThrowIfFailed(D3DReadFileToBlob(wstr.c_str(), &signature));
		ThrowIfFailed(pDevice->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&mpRootSignature)));
	}

	// Shaders
	{
		std::string str{ "../x64/" + dir + "/" + name + "_vs.cso" };
		std::wstring wstr(str.begin(), str.end());
		ThrowIfFailed(D3DReadFileToBlob(wstr.c_str(), &mVS));
	}
	{
		std::string str{ "../x64/" + dir + "/" + name + "_ps.cso" };
		std::wstring wstr(str.begin(), str.end());
		ThrowIfFailed(D3DReadFileToBlob(wstr.c_str(), &mPS));
	}

	// Input Layout
	{
		D3D12_INPUT_ELEMENT_DESC inputElementDescs[]
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};
		// Build Pipeline State
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc{};
		psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
		psoDesc.pRootSignature = mpRootSignature.Get();
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(mVS.Get());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(mPS.Get());
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState.DepthEnable = FALSE;
		psoDesc.DepthStencilState.StencilEnable = FALSE;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
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

	for (auto pViewC : mpViewCs)
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

void Material::AddViewC(ViewC* pViewC)
{
	mpViewCs.emplace_back(pViewC);
}
