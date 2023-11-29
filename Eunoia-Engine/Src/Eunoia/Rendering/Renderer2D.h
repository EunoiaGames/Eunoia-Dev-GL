#pragma once

#include "../Common.h"
#include "RenderAPI.h"
#include "../Math/Math.h"

namespace Eunoia {

	enum RendererSpritePosOrigin
	{
		EU_SPRITE_POS_ORIGIN_BOTTOM_LEFT,
		EU_SPRITE_POS_ORIGIN_TOP_LEFT,
		EU_SPRITE_POS_ORIGIN_TOP_RIGHT,
		EU_SPRITE_POS_ORIGIN_BOTTOM_RIGHT,
		EU_SPRITE_POS_ORIGIN_CENTER,

		EU_NUM_SPRITE_POS_ORIGIN_TYPES,
		EU_SPRITE_POS_ORIGIN_DEFAULT = EU_SPRITE_POS_ORIGIN_BOTTOM_LEFT
	};

	enum Renderer2DVecType
	{
		EU_RENDERER2D_VEC_AMBIENT,

		EU_NUM_RENDERER2D_VEC_TYPES
	};

	enum Renderer2DFloatType
	{
		EU_RENDERER2D_FLOAT_DARKNESS,

		EU_NUM_RENDERER2D_FLOAT_TYPES,
	};

	struct Light2D
	{
		v2 pos;
		v3 color;
	};

	class EU_API Renderer2D
	{
	public:
		virtual void Init(u32 maxSprites) = 0;
		virtual void Destroy() = 0;

		virtual void BeginFrame() = 0;
		virtual void SubmitSprite(const v2& pos, const v2& size, r32 rot, const v4& color = v4(1.0f, 1.0f, 1.0f, 1.0f),
			TextureID texture = EU_INVALID_TEXTURE_ID, b32 occluder = false) = 0;
		virtual void SubmitLight(v2 pos, v4 color) = 0;
		virtual void EndFrame() = 0;
		virtual void RenderFrame() = 0;

		virtual void SetSpritePosOrigin(RendererSpritePosOrigin) = 0;
		virtual void SetProjection(const m4& projection) = 0;
		virtual void SetCamera(v2 pos, r32 rot) = 0;

		virtual void ResizeFramebuffers(u32 width, u32 height) = 0;
		virtual TextureID GetOutputTexture() = 0;

		void SubmitSprite(const v2& pos, const v2& size, r32 rot, const v4& color, b32 occluder = false);
		void SubmitSprite(const v2& pos, const v2& size, r32 rot, TextureID texture, b32 occluder = false);
		void SubmitSprite(const v2& pos, const v2& size, const v4& color, b32 occluder = false);
		void SubmitSprite(const v2& pos, const v2& size, TextureID texture, b32 occluder = false);
		void SubmitSprite(const v2& pos, const v2& size, TextureID texture, const v4& color, b32 occluder = false);

		void SetVecValue(Renderer2DVecType type, const v4& value);
		void SetFloatValue(Renderer2DFloatType type, r32 value);
	protected:
		v4 m_VecValues[EU_NUM_RENDERER2D_VEC_TYPES];
		r32 m_FloatValues[EU_NUM_RENDERER2D_FLOAT_TYPES];
	public:
		static Renderer2D* CreateRenderer(RenderAPI renderAPI);
	};

}