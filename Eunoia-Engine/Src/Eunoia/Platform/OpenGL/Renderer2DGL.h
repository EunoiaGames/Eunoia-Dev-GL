#pragma once

#include "../../Rendering/Renderer2D.h"
#include "../../Rendering/RenderAPI.h"
#include "../../Math/Math.h"
#include "../../DataStructures/List.h"

#include <GL\glew.h>

#define EU_RENDERER2D_MAX_TEXTURES 32
#define EU_RENDERER2D_LIGHTMAP_SIZE 256

namespace Eunoia {

	struct Renderer2DVertex
	{
		v2 pos;
		v2 texCoord;
		v4 color;
		r32 texIndex;
	};

	struct Renderer2DOccluderVertex
	{
		v2 pos;
	};

	class EU_API Renderer2DGL : public Renderer2D
	{
	public:
		Renderer2DGL();
		virtual void Init(u32 maxSprites) override;
		virtual void Destroy() override;

		virtual void SetSpritePosOrigin(RendererSpritePosOrigin origin);
		virtual void SetProjection(const m4& projection);
		virtual void SetCamera(v2 pos, r32 rot);

		virtual void BeginFrame() override;
		virtual void SubmitSprite(const v2& pos, const v2& size, r32 rot, const v4& color = v4(1.0f, 1.0f, 1.0f, 1.0f),
			TextureID texture = EU_INVALID_TEXTURE_ID, b32 occluder = false) override;
		virtual void SubmitLight(v2 pos, v4 color) override;
		virtual void EndFrame() override;
		virtual void RenderFrame() override;

		virtual void ResizeFramebuffers(u32 width, u32 height) override;
		virtual TextureID GetOutputTexture() override;
	private:
		//void DoLightPass(const Light2D& light);
	private:
		
		GLuint m_Vao;
		GLuint m_Vbo;
		GLuint m_Ibo;
		ShaderID m_SpriteShader;

		TextureID m_WhiteTexture;
		TextureID m_Textures[EU_RENDERER2D_MAX_TEXTURES];
		u32 m_NumTextures;

		u32 m_SpriteCount;
		Renderer2DVertex* m_MappedVertex;

		m4 m_Projection;
		m4 m_View;
		RendererSpritePosOrigin m_Origin;

		GLuint m_FBO;
		TextureID m_OutputTexture;

		List<Light2D> m_Lights;
	};

}
