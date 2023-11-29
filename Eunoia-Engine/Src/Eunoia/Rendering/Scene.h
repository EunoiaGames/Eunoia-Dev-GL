#pragma once

#include "../ECS/ECS.h"
#include "AssetManager.h"

#define EU_INVALID_SCENE_ID 0

namespace Eunoia {

	typedef u32 SceneID;

	struct Scene
	{
		String name;
		String path;
		ECS* ecs;
		AssetManager* assetManager;
	};

}