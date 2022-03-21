#pragma once

namespace Ion
{
	namespace Core
	{
		class Canvas;

		struct CubeCanvasCamera
		{
			Canvas* mpCanvas;
			float mDot;
			float mDist;
		};
	}
}
