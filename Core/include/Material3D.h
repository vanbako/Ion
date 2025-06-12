#pragma once
#include "ViewCCube.h"
#include "TextureType.h"
#include "SemanticInfo.h"
#include <dxcapi.h>
#include "Material.h"

namespace Ion
{
	namespace Core
	{
		class Application;
		class Canvas;
		class ViewC;

		class Material3D final
			: public Material
		{
		public:
			explicit Material3D(Core::Application* pApplication, const std::string& name);
			~Material3D();
			Material3D(const Material3D& other) = delete;
			Material3D(Material3D&& other) noexcept = delete;
			Material3D& operator=(const Material3D& other) = delete;
			Material3D& operator=(Material3D&& other) noexcept = delete;

			void Initialize() override;
			void Render(Core::Canvas* pCanvas) override;
			void ViewCUpdate(Core::Canvas* pCanvas, float delta);
			void AddViewC(Core::Canvas* pCanvas, Core::ViewC* pViewC) override;

			const Microsoft::WRL::ComPtr<ID3D12RootSignature>& GetRootSignature();
			const Microsoft::WRL::ComPtr<ID3D12PipelineState>& GetPipelineState();
			D3D12_INPUT_ELEMENT_DESC* GetInputElementDescs();
			UINT GetInputElementCount() const;
			UINT GetLayoutSize() const;
			const std::set<Core::TextureType>& GetTextureTypeSet() const;

			static const std::unordered_map<std::string, Core::SemanticInfo>& GetSemanticStrings();
			void MoveViewC(Core::Canvas* pCanvas, Core::ViewC* pViewC, Core::ViewCCube* pCurrCube);
			void AddViewCToCube(std::multimap<long long, Core::ViewCCube>& cubes, Core::ViewC* pViewC);
		private:
			static Core::Vector<long long> mViewCCubeSize;
			static const UINT mMaxInputParam;
			static const std::unordered_map<std::string, Core::SemanticInfo> mSemanticStrings;
			static const std::unordered_map<std::string, Core::TextureType> mTextureTypeStrings;

			Microsoft::WRL::ComPtr<ID3DBlob>
				mRS,
				mVS,
				mPS;
			Microsoft::WRL::ComPtr<ID3D12RootSignature> mpRootSignature;
			Microsoft::WRL::ComPtr<ID3D12PipelineState> mpPipelineState;
			D3D12_SIGNATURE_PARAMETER_DESC mSigParDescs[(std::size_t)InputSemantic::Count];
                       D3D12_INPUT_ELEMENT_DESC* mpInputElementDescs;
                       std::vector<std::string> mInputSemanticNames;
			UINT
				mConstantBufferCount,
				mInputElementCount,
				mLayoutSize;
			std::unordered_map<Core::Canvas*, std::multimap<long long, Core::ViewCCube>> mpCanvasCubes;
			std::set<Core::TextureType> mTextureTypes;

			Core::Vector<long long> GetViewCCubePos(Core::ViewC* pViewC);
			std::multimap<long long, Core::ViewCCube>::iterator GetViewCCubeIterator(Core::Vector<long long>& cubePos, std::multimap<long long, Core::ViewCCube>& cubes);
		};
	}
}
