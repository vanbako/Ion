#pragma once
#include "Cube.h"
#include "TextureType.h"
#include "SemanticInfo.h"
#include <dxcapi.h>

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
			explicit Material3D(Core::Application* pApplication, const std::string& name);
			~Material3D();
			Material3D(const Material3D& other) = delete;
			Material3D(Material3D&& other) noexcept = delete;
			Material3D& operator=(const Material3D& other) = delete;
			Material3D& operator=(Material3D&& other) noexcept = delete;

			void Initialize();
			void Render(Core::Canvas* pCanvas);
			const Microsoft::WRL::ComPtr<ID3D12RootSignature>& GetRootSignature();
			const Microsoft::WRL::ComPtr<ID3D12PipelineState>& GetPipelineState();
			D3D12_INPUT_ELEMENT_DESC* GetInputElementDescs();
			UINT GetInputElementCount() const;
			UINT GetLayoutSize() const;
			const std::set<Core::TextureType>& GetTextureTypeSet() const;

			void AddViewC(Core::Canvas* pCanvas, Core::ViewC* pViewC);

			static const std::unordered_map<std::string, Core::SemanticInfo>& GetSemanticStrings();
			void MoveViewC(Core::Canvas* pCanvas, Core::ViewC* pViewC, Core::Cube* pCurrCube);
			void AddViewCToCube(std::multimap<long long, Core::Cube>& cubes, Core::ViewC* pViewC);
		private:
			static Core::Vector<long long> mCubeSize;
			static const UINT mMaxInputParam;
			static const std::unordered_map<std::string, Core::SemanticInfo> mSemanticStrings;
			static const std::unordered_map<std::string, Core::TextureType> mTextureTypeStrings;

			bool mIsInitialized;
			Core::Application* mpApplication;
			std::string mName;
			Microsoft::WRL::ComPtr<ID3DBlob>
				mRS,
				mVS,
				mPS;
			Microsoft::WRL::ComPtr<ID3D12RootSignature> mpRootSignature;
			Microsoft::WRL::ComPtr<ID3D12PipelineState> mpPipelineState;
			D3D12_SIGNATURE_PARAMETER_DESC mSigParDescs[(std::size_t)InputSemantic::Count];
			D3D12_INPUT_ELEMENT_DESC* mpInputElementDescs;
			UINT
				mConstantBufferCount,
				mInputElementCount,
				mLayoutSize;
			std::unordered_map<Core::Canvas*, std::multimap<long long, Core::Cube>> mpCanvasCubes;
			std::set<Core::TextureType> mTextureTypes;
		};
	}
}
