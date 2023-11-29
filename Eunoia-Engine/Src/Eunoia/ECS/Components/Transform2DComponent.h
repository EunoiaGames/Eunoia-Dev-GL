#pragma once

#include "../ECS.h"

namespace Eunoia {

	EU_REFLECT(Component)
	struct Transform2DComponent : public ECSComponent
	{
		EU_PROPERTY(EU_PROPERTY_NO_EDIT) Transform2D worldTransform;
		EU_PROPERTY() Transform2D localTransform;
	};

}