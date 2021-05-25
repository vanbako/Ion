#pragma once
#include "../Core/Helpers.h"
#include "../Core/d3dx12.h"

namespace Ion
{
	namespace Core
	{
		template<class T>
		class UploadBuffer
		{
		public:
			explicit UploadBuffer(ID3D12Device* pDevice, UINT elementCount, bool isConstantBuffer)
				: mpUploadBuffer{}
				, mpMappedData{ nullptr }
				, mIsConstantBuffer{ isConstantBuffer }
				, mElementByteSize{ sizeof(T) }
				, mElementCount{ elementCount }
			{
				if (mIsConstantBuffer)
					mElementByteSize = Helpers::CalcConstantBufferByteSize(sizeof(T));
				D3D12_HEAP_PROPERTIES heapPropsUpload{ CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD) };
				D3D12_RESOURCE_DESC resourceDesc{ CD3DX12_RESOURCE_DESC::Buffer(mElementByteSize * mElementCount) };
				pDevice->CreateCommittedResource(
					&heapPropsUpload,
					D3D12_HEAP_FLAG_NONE,
					&resourceDesc,
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(&mpUploadBuffer));
				mpUploadBuffer->Map(0, nullptr, (void**)&mpMappedData);
			}
			virtual ~UploadBuffer()
			{
				if (mpUploadBuffer != nullptr)
					mpUploadBuffer->Unmap(0, nullptr);
				mpMappedData = nullptr;
			}
			UploadBuffer(const UploadBuffer& other) = delete;
			UploadBuffer(UploadBuffer&& other) noexcept = delete;
			UploadBuffer& operator=(const UploadBuffer& other) = delete;
			UploadBuffer& operator=(UploadBuffer&& other) noexcept = delete;

			ID3D12Resource* GetResource() const
			{
				return mpUploadBuffer.Get();
			}
			void CopyData(int elementIndex, T* data)
			{
				std::memcpy(&mpMappedData[elementIndex * mElementByteSize], data, sizeof(T) * mElementCount);
			}
		private:
			Microsoft::WRL::ComPtr<ID3D12Resource> mpUploadBuffer;
			BYTE* mpMappedData;
			bool mIsConstantBuffer;
			UINT mElementByteSize;
			UINT mElementCount;
		};
	}
}
