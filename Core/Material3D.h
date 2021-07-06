#pragma once
#include "../Core/TextureType.h"
#include "../Core/SemanticInfo.h"

namespace Ion
{
	namespace Core
	{
		class Application;
		class Canvas;
		class ViewC;

		class Material3D final
		{
		public:
			explicit Material3D(Application* pApplication, const std::string& name);
			~Material3D();
			Material3D(const Material3D& other) = delete;
			Material3D(Material3D&& other) noexcept = delete;
			Material3D& operator=(const Material3D& other) = delete;
			Material3D& operator=(Material3D&& other) noexcept = delete;

			void Initialize();
			void Render(Canvas* pCanvas);
			const Microsoft::WRL::ComPtr<ID3D12RootSignature>& GetRootSignature();
			const Microsoft::WRL::ComPtr<ID3D12PipelineState>& GetPipelineState();
			D3D12_INPUT_ELEMENT_DESC* GetInputElementDescs();
			UINT GetInputElementCount() const;
			UINT GetLayoutSize() const;
			const std::set<TextureType>& GetTextureTypeSet() const;

			void AddViewC(Canvas* pCanvas, ViewC* pViewC);

			static const std::map<std::string, SemanticInfo>& GetSemanticStrings();
		private:
			static const UINT mMaxInputParam;
			static const std::map<std::string, SemanticInfo> mSemanticStrings;
			static const std::map<std::string, TextureType> mTextureTypeStrings;

			bool mIsInitialized;
			Application* mpApplication;
			std::string mName;
			Microsoft::WRL::ComPtr<ID3DBlob>
				mRS,
				mVS,
				mPS;
			Microsoft::WRL::ComPtr<ID3D12RootSignature> mpRootSignature;
			Microsoft::WRL::ComPtr<ID3D12PipelineState> mpPipelineState;
			D3D12_SIGNATURE_PARAMETER_DESC mSigParDescs[(size_t)InputSemantic::Count];
			D3D12_INPUT_ELEMENT_DESC* mpInputElementDescs;
			UINT
				mConstantBufferCount,
				mInputElementCount,
				mLayoutSize;
			std::map<Canvas*,std::vector<ViewC*>> mpCanvasViewCs;
			std::set<TextureType> mTextureTypes;
		};
	}
}
