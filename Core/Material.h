#pragma once

namespace Ion
{
	namespace Core
	{
		class Application;
		class Canvas;
		class ViewC;

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

			void AddViewC(ViewC* pViewC);
		private:
			bool mIsInitialized;
			Application* mpApplication;
			std::string mName;
			Microsoft::WRL::ComPtr<ID3DBlob>
				mVS,
				mPS;
			Microsoft::WRL::ComPtr<ID3D12RootSignature> mpRootSignature;
			Microsoft::WRL::ComPtr<ID3D12PipelineState> mpPipelineState;
			std::vector<ViewC*> mpViewCs;
		};
	}
}
