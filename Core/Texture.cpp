#include "pch.h"
#include "Texture.h"
#include "Application.h"
#include "WICTextureLoader.h"

using namespace Ion;

Core::Texture::Texture(Core::Application* pApplication, const std::string& name)
	: mpApplication{ pApplication }
	, mpTexture{}
{
	auto pDevice{ mpApplication->GetDevice() };
	auto pCmdQueue{ mpApplication->GetCommandQueue() };

	DirectX::ResourceUploadBatch uploadBatch{ pDevice.Get() };
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	std::wstring wName{ converter.from_bytes("../Resources/Texture/" + name)};
	std::unique_ptr<uint8_t[]> wicData{};
	uploadBatch.Begin();
	ThrowIfFailed(DirectX::CreateWICTextureFromFile(
		pDevice.Get(),
		uploadBatch,
		wName.c_str(),
		mpTexture.GetAddressOf()));
	
	std::future<void> uploadResourcesFinished{ uploadBatch.End(pCmdQueue.Get()) };
	uploadResourcesFinished.wait();
}

const Microsoft::WRL::ComPtr<ID3D12Resource>& Core::Texture::GetTexture() const
{
	return mpTexture;
}
