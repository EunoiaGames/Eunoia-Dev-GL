#include "Renderer2D.h"

#include "../Platform/OpenGL/Renderer2DGL.h"

namespace Eunoia {
	
	void Renderer2D::SubmitSprite(const v2& pos, const v2& size, r32 rot, const v4& color, b32 occluder)
	{
		SubmitSprite(pos, size, rot, color, EU_INVALID_TEXTURE_ID, occluder);
	}

	void Renderer2D::SubmitSprite(const v2& pos, const v2& size, r32 rot, TextureID texture, b32 occluder)
	{
		SubmitSprite(pos, size, rot, v4(1.0f, 1.0f, 1.0f, 1.0f), texture, occluder);
	}

	void Renderer2D::SubmitSprite(const v2& pos, const v2& size, const v4& color, b32 occluder)
	{
		SubmitSprite(pos, size, 0.0f, color, EU_INVALID_TEXTURE_ID, occluder);
	}

	void Renderer2D::SubmitSprite(const v2& pos, const v2& size, TextureID texture, b32 occluder)
	{
		SubmitSprite(pos, size, 0.0f, v4(1.0f, 1.0f, 1.0f, 1.0f), EU_INVALID_TEXTURE_ID, occluder);
	}

	void Renderer2D::SubmitSprite(const v2& pos, const v2& size, TextureID texture, const v4& color, b32 occluder)
	{
		SubmitSprite(pos, size, 0.0f, color, texture, occluder);
	}

	void Renderer2D::SetVecValue(Renderer2DVecType type, const v4& value)
	{
		m_VecValues[type] = value;
	}

	void Renderer2D::SetFloatValue(Renderer2DFloatType type, r32 value)
	{
		m_FloatValues[type] = value;
	}

	Renderer2D* Renderer2D::CreateRenderer(RenderAPI renderAPI)
	{
		switch (renderAPI)
		{
		case EU_RENDER_API_OPENGL: return new Renderer2DGL();
		}

		return EU_NULL;
	}

}
