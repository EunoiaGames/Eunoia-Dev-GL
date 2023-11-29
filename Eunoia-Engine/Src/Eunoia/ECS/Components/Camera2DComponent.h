#pragma once

#include "../ECS.h"

namespace Eunoia {

	EU_REFLECT()
	enum Camera2DOrthoType
	{
		EU_CAMERA_2D_STANDARD_ORTHO,
		EU_CAMERA_2D_ADJUSTED_ORTHO,
	};

	EU_REFLECT()
	struct Camera2DStandardOrtho
	{
		Camera2DStandardOrtho() :
			left(-1.0f),
			right(1.0f),
			bottom(-1.0f),
			top(1.0f)
		{}

		EU_PROPERTY() r32 left;
		EU_PROPERTY() r32 right;
		EU_PROPERTY() r32 bottom;
		EU_PROPERTY() r32 top;
	};

	EU_REFLECT()
	struct Camera2DAdjustedOrtho
	{
		Camera2DAdjustedOrtho() :
			orthoScale(1.0f),
			orthoAdjusted(0.0f)
		{}

		EU_PROPERTY() r32 orthoScale;
		EU_PROPERTY() r32 orthoAdjusted;
	};

	EU_REFLECT(Component)
	struct Camera2DComponent : public ECSComponent
	{
		Camera2DComponent() :
			type(EU_CAMERA_2D_STANDARD_ORTHO)
		{}

		EU_PROPERTY() Camera2DOrthoType type;
		EU_PROPERTY() Camera2DStandardOrtho standard;
		EU_PROPERTY() Camera2DAdjustedOrtho adjusted;
	};

}