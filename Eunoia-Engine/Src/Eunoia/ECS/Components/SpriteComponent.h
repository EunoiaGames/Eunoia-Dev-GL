#pragma once

#include "../ECS.h"
#include "../../Rendering/RenderObjectIDs.h"

namespace Eunoia {

	EU_REFLECT(Component)
	struct SpriteComponent : public ECSComponent
	{
		SpriteComponent(v2 size = v2(0, 0), v4 color = v4(1.0f, 1.0f, 1.0f, 1.0f), TextureID texture = EU_INVALID_TEXTURE_ID, b32 occluder = false) :
			size(size),
			color(color),
			texture(texture),
			occluder(occluder)
		{}

		EU_PROPERTY() v2 size;
		EU_PROPERTY() v4 color;
		EU_PROPERTY() TextureID texture;
		EU_PROPERTY(EU_PROPERTY_BOOL32) b32 occluder;
	};


}
