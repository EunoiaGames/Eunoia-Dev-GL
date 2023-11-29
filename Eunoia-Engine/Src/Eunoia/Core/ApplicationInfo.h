#pragma once

#include "../DataStructures/String.h"
#include "../Rendering/DisplayInfo.h"

namespace Eunoia {

	struct Version
	{
		u32 major;
		u32 minor;
		u32 patch;
	};

	struct ApplicationInfo
	{
		String name;
		Version version;
		DisplayInfo displayInfo;
	};

}