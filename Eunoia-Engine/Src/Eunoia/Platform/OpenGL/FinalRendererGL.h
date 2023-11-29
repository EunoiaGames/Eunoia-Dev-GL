#pragma once

#include "../../Rendering/FinalRenderer.h"
#include "Renderer2DGL.h"

namespace Eunoia {

	class FinalRendererGL : public FinalRenderer
	{
	public:
		FinalRendererGL();
		virtual void Init(u32 maxSprites = 10000, b32 renderToSwapchain = false) override;
		virtual void BeginFrame() override;
		virtual void EndFrame() override;
		virtual void RenderFrame() override;

		virtual void ResizeOutput(u32 width, u32 height) override;

		TextureID GetOutputTexture() override;

		virtual Renderer2D* GetRenderer2D();

	private:
		Renderer2DGL* m_Renderer2D;
		GLuint m_Vao;
		GLuint m_Vbo;
		GLuint m_Ibo;
		ShaderID m_Shader;
		//used if not rendering to swapchain
		u32 m_FBO;
		TextureID m_OutputTexture;
		b32 m_RenderingToFBO;
		////////////////////////////////////

	};

}