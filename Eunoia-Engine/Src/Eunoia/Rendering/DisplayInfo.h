#pragma once

#include "../DataStructures/String.h"

namespace Eunoia {

	enum DisplayResolution
	{
		EU_DISPLAY_RESOLUTION_16_9,
		EU_DISPLAY_RESOLUTION_4_3,
	};

	struct DisplayInfo
	{
		String title;
		DisplayResolution res;
		u32 resScale;
	};

}