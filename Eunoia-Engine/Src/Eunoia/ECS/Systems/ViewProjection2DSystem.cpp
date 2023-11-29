#include "ViewProjection2DSystem.h"

#include "../Components/Camera2DComponent.h"
#include "../Components/Transform2DComponent.h"
#include "../../Core/Engine.h"
#include "../../Rendering/Renderer2D.h"

namespace Eunoia {

	ViewProjection2DSystem::ViewProjection2DSystem()
	{
		AddComponentType<Transform2DComponent>();
		AddComponentType<Camera2DComponent>();
	}

	void ViewProjection2DSystem::ProcessEntityOnRender(EntityID entity)
	{
		Camera2DComponent* cameraComponent = m_ECS->GetComponent<Camera2DComponent>(entity);
		Transform2DComponent* transformComponent = m_ECS->GetComponent<Transform2DComponent>(entity);
		const Transform2D& transform = transformComponent->worldTransform;

		Renderer2D* renderer = Engine::GetRenderer2D();

		v2 cameraPos = transform.pos * -1.0f;
		r32 cameraRot = transform.rot * -1.0f;

		switch (cameraComponent->type)
		{
		case EU_CAMERA_2D_STANDARD_ORTHO: {
			const Camera2DStandardOrtho& standard = cameraComponent->standard;
			renderer->SetProjection(m4::CreateOrthographic(standard.left, standard.right, standard.bottom, standard.top, 0.0f, 1000.0f));
		} break;
		case EU_CAMERA_2D_ADJUSTED_ORTHO: {
			const Camera2DAdjustedOrtho& adjusted = cameraComponent->adjusted;
			v2 size = Engine::GetOutputSize();
			r32 width = size.x;
			r32 height = size.y;

			r32 hw = width / 2.0f;
			r32 hh = height / 2.0f;
			r32 ar = hw / hh;

			r32 left = -hw;
			r32 right = hw;
			r32 bottom = hh;
			r32 top = -hh;

			renderer->SetProjection(m4::CreateOrthographic(left, right, bottom, top, 0.0f, 1000.0f));

		}break;
		}

		renderer->SetCamera(cameraPos, cameraRot);
	}

}
