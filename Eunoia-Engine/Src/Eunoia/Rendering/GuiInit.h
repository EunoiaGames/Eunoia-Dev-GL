#pragma once

#include "../Common.h"

namespace Eunoia {

	class EU_API GuiInit
	{
	public:
		static void Init();
		static void Begin();
		static void End();
		static void Render();
	};

}