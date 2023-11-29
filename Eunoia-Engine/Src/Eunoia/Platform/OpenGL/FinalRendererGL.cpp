#include "FinalRendererGL.h"
#include "../../Core/Engine.h"
#include "../../Platform/OpenGL/RenderAPIOpenGL.h"

namespace Eunoia {

	static void DisplayCallback(const DisplayEvent& e, void* userPtr)
	{
		if (e.type == DISPLAY_EVENT_RESIZE)
		{
			
		}
	}

	FinalRendererGL::FinalRendererGL() :
		m_RenderingToFBO(false)
	{
	}

	void FinalRendererGL::Init(u32 maxSprites, b32 renderToSwapchain)
	{
		m_RenderingToFBO = !renderToSwapchain;

		Engine::GetDisplay()->AddDisplayEventCallback(DisplayCallback, this);

		m_Renderer2D = new Renderer2DGL();
		m_Renderer2D->Init(maxSprites);

		glGenVertexArrays(1, &m_Vao);
		glGenBuffers(1, &m_Vbo);
		glGenBuffers(1, &m_Ibo);

		glBindVertexArray(m_Vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo);

		v2 vertices[] = { {-1.0f, -1.0f}, {-1.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, -1.0f} };
		u32 indices[] = { 0, 1, 2, 0, 2, 3 };

		glBufferData(GL_ARRAY_BUFFER, sizeof(v2) * 4, vertices, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * 6, indices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(v2), 0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		m_Shader = Engine::GetOpenGL()->CreateShader("Res/Shaders/Final.glsl");

		if (!renderToSwapchain)
		{
			u32 width, height;
			Engine::GetDisplay()->GetSize(&width, &height);

			glGenFramebuffers(1, &m_FBO);
			glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
			glGenTextures(1, &m_OutputTexture);
			glBindTexture(GL_TEXTURE_2D, m_OutputTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, EU_NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_OutputTexture, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, 0);

			Engine::GetOpenGL()->AddFramebufferAttachmentTexture(m_OutputTexture, width, height);
		}
	}

	void FinalRendererGL::BeginFrame()
	{
		m_Renderer2D->BeginFrame();
	}

	void FinalRendererGL::EndFrame()
	{
		m_Renderer2D->EndFrame();
	}

	void FinalRendererGL::RenderFrame()
	{
		m_Renderer2D->RenderFrame();
		TextureID output2D = m_Renderer2D->GetOutputTexture();

		RenderAPIOpenGL* openGL = Engine::GetOpenGL();

		openGL->BindShader(m_Shader);
		openGL->BindTextureToShader("Output2D", output2D, 0);
		
		u32 width, height;

		if (m_RenderingToFBO)
		{
			Engine::GetOpenGL()->GetTextureSize(m_OutputTexture, &width, &height);
			glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		}
		else
		{
			Engine::GetDisplay()->GetSize(&width, &height);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glViewport(0, 0, width, height);
		glBindVertexArray(m_Vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo);
		glEnableVertexAttribArray(0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, EU_NULL);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		if (m_RenderingToFBO)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}

	void FinalRendererGL::ResizeOutput(u32 width, u32 height)
	{
		if (m_RenderingToFBO)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
			glBindTexture(GL_TEXTURE_2D, m_OutputTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, EU_NULL);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
			TextureGL* tex = Engine::GetOpenGL()->GetTextureGL(m_OutputTexture);
			tex->width = width;
			tex->height = height;
		}

		m_Renderer2D->ResizeFramebuffers(width, height);
	}

	TextureID FinalRendererGL::GetOutputTexture()
	{
		return m_OutputTexture;
	}

	Renderer2D* FinalRendererGL::GetRenderer2D()
	{
		return m_Renderer2D;
	}

}
