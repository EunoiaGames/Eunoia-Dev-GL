#include "SpriteSubmissionSystem.h"
#include "../Components/Transform2DComponent.h"
#include "../Components/SpriteComponent.h"
#include "../../Core/Engine.h"
#include "../../Rendering/Renderer2D.h"

namespace Eunoia {

	SpriteSubmissionSystem::SpriteSubmissionSystem()
	{
		AddComponentType<Transform2DComponent>();
		AddComponentType<SpriteComponent>();
	}

	void SpriteSubmissionSystem::ProcessEntityOnRender(EntityID entity)
	{
		const Transform2D& transform = m_ECS->GetComponent<Transform2DComponent>(entity)->worldTransform;
		SpriteComponent* sprite = m_ECS->GetComponent<SpriteComponent>(entity);

		Engine::GetRenderer2D()->SubmitSprite(transform.pos, sprite->size, sprite->texture, sprite->color);
	}

}
