#pragma once
#include "InputSemantic.h"

namespace Ion
{
	namespace Core
	{
		class Application;
		class Canvas;
		class ViewC;

		struct SemanticInfo
		{
			InputSemantic inputSemantic;
			DXGI_FORMAT format;
			UINT offset;
		};

		class Material final
		{
		public:
			explicit Material(Application* pApplication, const std::string& name);
			~Material();
			Material(const Material& other) = delete;
			Material(Material&& other) noexcept = delete;
			Material& operator=(const Material& other) = delete;
			Material& operator=(Material&& other) noexcept = delete;

			void Initialize();
			void Render(Canvas* pCanvas);
			const Microsoft::WRL::ComPtr<ID3D12RootSignature>& GetRootSignature();
			const Microsoft::WRL::ComPtr<ID3D12PipelineState>& GetPipelineState();
			D3D12_INPUT_ELEMENT_DESC* GetInputElementDescs();
			UINT GetInputElementCount() const;
			UINT GetLayoutSize() const;

			void AddViewC(Canvas* pCanvas, ViewC* pViewC);

			static const std::map<std::string, SemanticInfo>& GetSemanticStrings();
		private:
			static const UINT mMaxInputParam;
			static const std::map<std::string, SemanticInfo> mSemanticStrings;

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
		};
	}
}
