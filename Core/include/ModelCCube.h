#pragma once
#include "Cube.h"

namespace Ion
{
	namespace Core
	{
		class ModelC;

		struct CubeModelC
		{
			bool mHasMoved;
			ModelC* mpModelC;
		};

		class ModelCCube
			: public Cube
		{
		public:
			explicit ModelCCube(const Core::Vector<long long>& location);
			explicit ModelCCube();
			~ModelCCube() = default;
			ModelCCube(const ModelCCube& other) = delete;
			ModelCCube(ModelCCube&& other) noexcept = delete;
			ModelCCube& operator=(const ModelCCube& other) = delete;
			ModelCCube& operator=(ModelCCube&& other) noexcept = delete;

			void SetHasMoved(Core::ModelC* pModelC, bool hasMoved);
			void AddModelC(bool hasMoved, Core::ModelC* pModelC);
			void AddModelCCheckExistence(bool hasMoved, Core::ModelC* pModelC);
			void ModelCUpdate(float delta);
			void ModelCSwitch();
		private:
			static const float mModelUpdateDist;
			std::vector<CubeModelC> mCubeModelCs;
		};
	}
}
