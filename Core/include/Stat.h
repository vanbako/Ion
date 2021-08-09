#pragma once

#ifdef ION_STATS

namespace Ion
{
	namespace Core
	{
		enum class Stat
		{
			Input,
			Update,
			Switch,
			Render,
			Count
		};
	}
}

#endif