#include "../Core/pch.h"
#include "../Core/Texture.h"
#include "../Core/Application.h"
#include "../Core/WICTextureLoader.h"

using namespace Ion::Core;

Texture::Texture(Application* pApplication, const std::string& name)
	: mIsInitialized{ false }
	, mpApplication{ pApplication }
	, mpTexture{}
	, mUploadBatch{ pApplication->GetDevice().Get() }

{
	auto pDevice{ mpApplication->GetDevice() };
	auto pCmdQueue{ mpApplication->GetCommandQueue() };

	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	std::wstring wName{ converter.from_bytes("../Resources/Texture/" + name)};
	std::unique_ptr<uint8_t[]> wicData{};
	//ThrowIfFailed(DirectX::LoadWICTextureFromFile(
	//	pApplication->GetDevice().Get(),
	//	wName.c_str(),
	//	mpTexture.GetAddressOf(),
	//	wicData,
	//	mSubResourceData));
	mUploadBatch.Begin();
	ThrowIfFailed(DirectX::CreateWICTextureFromFile(
		pDevice.Get(),
		mUploadBatch,
		wName.c_str(),
		mpTexture.GetAddressOf()));
	
	auto uploadResourcesFinished{ mUploadBatch.End(pCmdQueue.Get()) };
	uploadResourcesFinished.wait();
}

const Microsoft::WRL::ComPtr<ID3D12Resource>& Texture::GetTexture() const
{
	return mpTexture;
}
