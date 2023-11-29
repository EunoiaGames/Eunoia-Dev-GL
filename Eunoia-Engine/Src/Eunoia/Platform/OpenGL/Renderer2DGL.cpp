#include "Renderer2DGL.h"

#include "../../Core/Engine.h"
#include "../../Math/Math.h"
#include "RenderAPIOpenGL.h"
#include "../../Utils/Logger.h"

namespace Eunoia {

	Renderer2DGL::Renderer2DGL() :
		m_NumTextures(0),
		m_Origin(EU_SPRITE_POS_ORIGIN_DEFAULT),
		m_SpriteCount(0)
	{
		
	}

	void Eunoia::Renderer2DGL::Init(u32 maxSprites)
	{
		u32 width, height;
		Engine::GetDisplay()->GetSize(&width, &height);
		r32 w = width;
		r32 h = height;
		r32 hw = width / 2.0f;
		r32 hh = height / 2.0f;
		m_Projection = m4::CreateIdentity();
		m_View = m4::CreateIdentity();

		m_VecValues[EU_RENDERER2D_VEC_AMBIENT] = v4(1.0f, 1.0f, 1.0f, 1.0f);
		m_FloatValues[EU_RENDERER2D_FLOAT_DARKNESS] = 1.0f;

		glGenVertexArrays(1, &m_Vao);
		glGenBuffers(1, &m_Vbo);
		glGenBuffers(1, &m_Ibo);

		glBindVertexArray(m_Vao);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo);

		mem_size vertexBufferSize = maxSprites * 4 * sizeof(Renderer2DVertex);
		mem_size indexBufferSize = maxSprites * 6 * sizeof(Renderer2DVertex);

		u32* Indices = (u32*)malloc(indexBufferSize);

		u32 Counter = 0;
		for (u32 i = 0; i < maxSprites * 6; i += 6)
		{
			Indices[i + 0] = Counter + 0;
			Indices[i + 1] = Counter + 1;
			Indices[i + 2] = Counter + 2;

			Indices[i + 3] = Counter + 0;
			Indices[i + 4] = Counter + 2;
			Indices[i + 5] = Counter + 3;

			Counter += 4;
		}

		glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, 0, GL_DYNAMIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, Indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Renderer2DVertex), (const GLvoid*)offsetof(Renderer2DVertex, Renderer2DVertex::pos));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Renderer2DVertex), (const GLvoid*)offsetof(Renderer2DVertex, Renderer2DVertex::texCoord));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Renderer2DVertex), (const GLvoid*)offsetof(Renderer2DVertex, Renderer2DVertex::color));
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Renderer2DVertex), (const GLvoid*)offsetof(Renderer2DVertex, Renderer2DVertex::texIndex));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		m_SpriteShader = Engine::GetRenderAPI()->CreateShader("Res/Shaders/Batch2D.glsl");

		u8 Pixels[4] = { 255, 255, 255, 255 };
		m_WhiteTexture = Engine::GetRenderAPI()->CreateTexture(Pixels, 1, 1);
		m_Textures[0] = m_WhiteTexture;
		m_NumTextures = 1;


		free(Indices);
		//FBO creation
		GLuint outputTexture;
		glGenFramebuffers(1, &m_FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		glGenTextures(1, &outputTexture);
		glBindTexture(GL_TEXTURE_2D, outputTexture);
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, EU_NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outputTexture, 0);

		GLint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			EU_LOG_ERROR("FB Incomplete");
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		m_OutputTexture = Engine::GetOpenGL()->AddFramebufferAttachmentTexture(outputTexture, width, height);
	}


	void Renderer2DGL::Destroy()
	{
		glDeleteVertexArrays(1, &m_Vao);
		glDeleteBuffers(1, &m_Vbo);
		glDeleteBuffers(1, &m_Ibo);
		glDeleteFramebuffers(1, &m_FBO);
	}

	void Renderer2DGL::SetSpritePosOrigin(RendererSpritePosOrigin origin)
	{
		m_Origin = origin;
	}

	void Renderer2DGL::SetProjection(const m4& projection)
	{
		m_Projection = projection;
	}

	void Renderer2DGL::SetCamera(v2 pos, r32 rot)
	{
	}

	void Renderer2DGL::BeginFrame()
	{
		m_SpriteCount = 0;
		m_NumTextures = 1;
		m_Lights.Clear();
		glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
		m_MappedVertex = (Renderer2DVertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

		RenderAPIOpenGL* openGL = Engine::GetOpenGL();

		m4 viewProjection = m_View * m_Projection;

		openGL->BindShader(m_SpriteShader);
		openGL->SetUniform("ViewProjection", viewProjection);

		v3 ambientRGB = m_VecValues[EU_RENDERER2D_VEC_AMBIENT].xyz() * m_FloatValues[EU_RENDERER2D_FLOAT_DARKNESS];
		v4 ambient = v4(ambientRGB, m_VecValues[EU_RENDERER2D_VEC_AMBIENT].w);

		openGL->SetUniform("Ambient", ambient);
	}

	void Renderer2DGL::SubmitSprite(const v2& pos, const v2& size, r32 rot, const v4& color, TextureID texture, b32 occluder)
	{
		v2 adjustedPos = pos;

		switch (m_Origin)
		{
		case EU_SPRITE_POS_ORIGIN_TOP_LEFT: adjustedPos = v2(adjustedPos.x, adjustedPos.y - size.y);
		case EU_SPRITE_POS_ORIGIN_BOTTOM_LEFT: break;
		case EU_SPRITE_POS_ORIGIN_BOTTOM_RIGHT: adjustedPos = v2(adjustedPos.x - size.x, adjustedPos.y); break;
		case EU_SPRITE_POS_ORIGIN_TOP_RIGHT: adjustedPos = adjustedPos - size; break;
		case EU_SPRITE_POS_ORIGIN_CENTER: adjustedPos = adjustedPos - size * 0.5f; break;
		}

		v2 TexCoord_bl(0.0f, 1.0f);
		v2 TexCoord_tl(0.0f, 0.0f);
		v2 TexCoord_tr(1.0f, 0.0f);
		v2 TexCoord_br(1.0f, 1.0f);

		Renderer2DVertex*& vertex = m_MappedVertex;

		u32 TextureIndex = 0;
		if (texture != EU_INVALID_TEXTURE_ID)
		{
			for (u32 i = 0; i < m_NumTextures; i++)
			{
				if (texture == m_Textures[i])
					TextureIndex = i;
			}

			if (TextureIndex == 0)
			{
				m_Textures[m_NumTextures] = texture;
				TextureIndex = m_NumTextures++;
			}
		}

		m3 transform = rot == 0 ? m3::CreateIdentity() : m3::CreateRotation(rot);

		v2 pos1 = transform * v2(0.0f, 0.0f) + adjustedPos;
		v2 pos2 = transform * v2(0.0f, size.y) + adjustedPos;
		v2 pos3 = transform * size + adjustedPos;
		v2 pos4 = transform * v2(size.x, 0.0f) + adjustedPos;

		vertex->color = color;
		vertex->texCoord = TexCoord_bl;
		vertex->pos = pos1;
		vertex->texIndex = TextureIndex;
		vertex++;

		vertex->color = color;
		vertex->texCoord = TexCoord_tl;
		vertex->pos = pos2;
		vertex->texIndex = TextureIndex;
		vertex++;

		vertex->color = color;
		vertex->texCoord = TexCoord_tr;
		vertex->pos = pos3;
		vertex->texIndex = TextureIndex;
		vertex++;

		vertex->color = color;
		vertex->texCoord = TexCoord_br;
		vertex->pos = pos4;
		vertex->texIndex = TextureIndex;
		vertex++;

		m_SpriteCount++;

		if (m_NumTextures == EU_RENDERER2D_MAX_TEXTURES)
		{
			EndFrame();
			RenderFrame();
			m_SpriteCount = 0;
			m_NumTextures = 0;
			glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
			m_MappedVertex = (Renderer2DVertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		}
	}

	void Renderer2DGL::SubmitLight(v2 pos, v4 color)
	{
		Light2D light;
		light.pos = pos;
		light.color = color.xyz();
		m_Lights.Push(light);
	}

	void Renderer2DGL::EndFrame()
	{

	}

	void Renderer2DGL::RenderFrame()
	{
		RenderAPIOpenGL* openGL = Engine::GetOpenGL();

		openGL->BindShader(m_SpriteShader);

		u32 width, height;
		Engine::GetOpenGL()->GetTextureSize(m_OutputTexture, &width, &height);

		for (u32 i = 0; i < m_NumTextures; i++)
			openGL->BindTextureToShader("Textures", m_Textures[i], i);

		glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
		glUnmapBuffer(GL_ARRAY_BUFFER);

		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glViewport(0.0f, 0.0f, width, height);
		
		glBindVertexArray(m_Vao);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo);
		glDrawElements(GL_TRIANGLES, m_SpriteCount * 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDisable(GL_BLEND);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	void Renderer2DGL::ResizeFramebuffers(u32 width, u32 height)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		glBindTexture(GL_TEXTURE_2D, m_OutputTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, EU_NULL);
		TextureGL* tex = Engine::GetOpenGL()->GetTextureGL(m_OutputTexture);
		tex->width = width;
		tex->height = height;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		r32 hw = width / 2.0f;
		r32 hh = height / 2.0f;
		//m_Projection = m4::CreateOrthographic(-hw, hw, -hh, hh, 0.0f, 1000.0f);
	}

	TextureID Renderer2DGL::GetOutputTexture()
	{
		return m_OutputTexture;
	}

}
