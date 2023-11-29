#pragma once

#include "../ECS.h"

namespace Eunoia {

	EU_REFLECT(System)
	class SpriteSubmissionSystem : public ECSSystem
	{
	public:
		SpriteSubmissionSystem();
		virtual void ProcessEntityOnRender(EntityID entity) override;
	};

}