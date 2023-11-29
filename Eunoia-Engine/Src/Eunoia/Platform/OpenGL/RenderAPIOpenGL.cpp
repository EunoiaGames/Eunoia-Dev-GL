#include "RenderAPIOpenGL.h"

#include "../../Rendering/Display.h"
#include "../../Utils/Logger.h"
#include "../../Utils/FileUtils.h"
#include "../../Core/Engine.h"
#include "../../Rendering/TextureLoader.h"

#include <GL\glew.h>
#ifdef EU_PLATFORM_WINDOWS
#include <GL\wglew.h>
#include "../Windows/DisplayWin32.h"
#endif

#include <GLFW\glfw3.h>
#include "../../Platform/GLFW/DisplayGLFW.h"

namespace Eunoia {

	static void DisplayCallback(const DisplayEvent& e, void* userPtr)
	{
		if (e.type == DISPLAY_EVENT_RESIZE)
		{
			glViewport(0, 0, e.width, e.height);
		}
	}

	void RenderAPIOpenGL::Init(Display* display)
	{
		if (display->GetDisplayAPI() == EU_DISPLAY_API_WIN32)
		{
			if (wglewInit() != GLEW_OK)
			{
				EU_LOG_ERROR("Could not initialize WGLEW");
			}

			HDC deviceContext = GetDC(((DisplayWin32*)display)->GetHandle());
			m_GLRC = wglCreateContext(deviceContext);
			wglMakeCurrent(deviceContext, m_GLRC);
			u32 width, height;
			display->GetSize(&width, &height);

			if (!m_GLRC)
			{
				EU_LOG_ERROR("Could not create Opengl Context");
				return;
			}

			if (glewInit() != GLEW_OK)
			{
				EU_LOG_ERROR("Could not initialize GLEW");
				return;
			}

			glViewport(0, 0, width, height);

			EU_LOG_INFO("Created OpenGL Context");
		}
		else if (display->GetDisplayAPI() == EU_DISPLAY_API_GLFW)
		{
			glfwMakeContextCurrent(((DisplayGLFW*)display)->GetHandle());

			if (glewInit() != GLEW_OK)
			{
				EU_LOG_ERROR("Could not initialize GLEW");
				return;
			}

			glfwSwapInterval(1);
			EU_LOG_INFO("Created OpenGL Context");
		}

		display->AddDisplayEventCallback(DisplayCallback, this);
	}

	void RenderAPIOpenGL::Destroy()
	{
		wglDeleteContext(m_GLRC);
	}

	void RenderAPIOpenGL::BeginFrame()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void RenderAPIOpenGL::EndFrame()
	{
		
	}

	TextureID RenderAPIOpenGL::CreateTexture(const u8* pixels, u32 width, u32 height, TextureFilter filter, TextureAddressMode addressMode)
	{
		GLuint textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		GLenum filterGL = GetTextureFilter(filter);
		GLint addresesModeGL = GetTextureAddressMode(addressMode);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, addresesModeGL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, addresesModeGL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterGL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterGL);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

		glBindTexture(GL_TEXTURE_2D, 0);

		TextureGL texture;
		texture.id = textureID;
		texture.width = width;
		texture.height = height;

		if (m_FreeTextureIDs.Empty())
		{
			m_Textures.Push(texture);
			return m_Textures.Size();
		}
		
		TextureID tID = m_FreeTextureIDs.GetLastElement();
		m_FreeTextureIDs.Pop();

		m_Textures[tID - 1] = texture;
		return tID;
	}

	TextureID RenderAPIOpenGL::CreateTexture(const String& path, TextureFilter filter, TextureAddressMode addressMode)
	{
		u32 width, height;
		u8* pixels = TextureLoader::LoadEutexTexture(path.C_Str(), &width, &height);

		TextureID texID = CreateTexture(pixels, width, height, filter, addressMode);
		m_Textures[texID - 1].path = path;
		return texID;
	}

	TextureGL* RenderAPIOpenGL::GetTextureGL(TextureID texture)
	{
		return &m_Textures[texture - 1];
	}

	void RenderAPIOpenGL::GetTextureSize(TextureID texture, u32* width, u32* height)
	{
		TextureGL* tex = &m_Textures[texture - 1];
		*width = tex->width;
		*height = tex->height;
	}

	void RenderAPIOpenGL::DestroyTexture(TextureID texture)
	{
		TextureGL* tex = &m_Textures[texture - 1];
		glDeleteTextures(1, &tex->id);
		m_FreeTextureIDs.Push(texture);
	}

	void RenderAPIOpenGL::BindTexture(TextureID texture, TextureUnit unit)
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		TextureGL* texGL = &m_Textures[texture - 1];
		glBindTexture(GL_TEXTURE_2D, texGL->id);
	}

	GLuint RenderAPIOpenGL::GetTextureHandleGL(TextureID texture)
	{
		return m_Textures[texture - 1].id;
	}

	static void ParseShader(ShaderInfo* shader, const String& src)
	{
		const String UNIFORM_KEYWORD = "uniform";

		TextureUnit currentUnit = 0;

		s32 uniformLocation = src.FindFirstOf(UNIFORM_KEYWORD);
		while (uniformLocation != -1)
		{
			s32 spaceIndex = uniformLocation + UNIFORM_KEYWORD.Length();
			s32 semicolonIndex = src.FindFirstOf(";", spaceIndex);
			String typeAndName = src.SubString(spaceIndex, semicolonIndex - 1);
			typeAndName = typeAndName.Trim();
			spaceIndex = typeAndName.FindFirstOf(" ");
			String type = typeAndName.SubString(0, spaceIndex - 1).TrimEnding();
			String name = typeAndName.SubString(spaceIndex + 1).TrimBeginning();

			s32 OpenBracket = name.FindFirstOf("[");
			s32 CloseBracket = name.FindFirstOf("]", OpenBracket + 1);

			u32 ArrayLength = 1;
			if (OpenBracket != -1 && CloseBracket != -1)
			{
				String ArrayLengthString = name.SubString(OpenBracket + 1, CloseBracket - 1).Trim();
				ArrayLength = ArrayLengthString.ParseInt();
				name = name.SubString(0, OpenBracket - 1);
			}

			if (type == "sampler2D")
			{
				ShaderSamplerInfo samplerInfo;
				samplerInfo.unit = currentUnit;
				samplerInfo.arrayLength = ArrayLength;
				samplerInfo.locations.SetCapacityAndElementCount(ArrayLength);

				if (ArrayLength > 1)
				{
					for (u32 i = 0; i < ArrayLength; i++)
					{
						String ElementName = name + "[" + String::S32ToString(i) + "]";
						u32 Location = glGetUniformLocation(shader->program, ElementName.C_Str());
						samplerInfo.locations[i] = Location;
					}
				}
				else
				{
					u32 Location = glGetUniformLocation(shader->program, name.C_Str());
					samplerInfo.locations[0] = Location;
				}

				shader->samplers[name] = samplerInfo;
				currentUnit += ArrayLength;
			}
			else
			{
				ShaderUniformInfo uniformInfo;
				uniformInfo.arrayLength = ArrayLength;
				uniformInfo.locations.SetCapacityAndElementCount(ArrayLength);

				if (ArrayLength > 1)
				{
					for (u32 i = 0; i < ArrayLength; i++)
					{
						String ElementName = name + "[" + String::S32ToString(i) + "]";
						u32 Location = glGetUniformLocation(shader->program, ElementName.C_Str());
						uniformInfo.locations[i] = Location;
					}
				}
				else
				{
					u32 Location = glGetUniformLocation(shader->program, name.C_Str());
					uniformInfo.locations[0] = Location;
				}

				shader->uniforms[name] = uniformInfo;
			}

			uniformLocation = src.FindFirstOf("uniform", uniformLocation + 1);
		}
	}

	static b32 ShaderCheckForError(GLuint Shader, GLenum Type, b32 IsProgram)
	{
		GLint Success = true;
		if (IsProgram)
		{
			glGetProgramiv(Shader, Type, &Success);
			if (!Success)
			{
				printf("Linking error\n");
			}
		}
		else
		{
			glGetShaderiv(Shader, Type, &Success);
			if (!Success)
			{
				GLint LogSize = 0;
				glGetShaderiv(Shader, GL_INFO_LOG_LENGTH, &LogSize);
				String ErrorMsg(LogSize);

				GLsizei Length;
				glGetShaderInfoLog(Shader, LogSize, &Length, ErrorMsg.GetChars());
				printf(ErrorMsg.C_Str());
				printf("\n");
			}
		}

		return Success;
	}

	ShaderID RenderAPIOpenGL::CreateShader(const String& path)
	{
		String ShaderText = FileUtils::LoadTextFile(path);

		s32 VertexIndex = ShaderText.FindFirstOf("#EU_Vertex", 0);
		s32 FragmentIndex = ShaderText.FindFirstOf("#EU_Fragment", VertexIndex);

		if(VertexIndex == -1 || FragmentIndex == -1)
		{
			EU_LOG_WARN("Could not find either #EU_Vertex or #EU_Fragment in shaders");
			return EU_INVALID_SHADER_ID;
		}

		VertexIndex += 10;
		String VertexText = ShaderText.SubString(VertexIndex, FragmentIndex - 1);
		FragmentIndex += 12;
		String FragmentText = ShaderText.SubString(FragmentIndex);

		ShaderInfo shaderInfo;
		shaderInfo.vertexShader = glCreateShader(GL_VERTEX_SHADER);
		shaderInfo.fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		shaderInfo.program = glCreateProgram();

		c_string VertexCode = VertexText.C_Str();
		c_string FragmentCode = FragmentText.C_Str();

		glShaderSource(shaderInfo.vertexShader, 1, &VertexCode, 0);
		glShaderSource(shaderInfo.fragmentShader, 1, &FragmentCode, 0);

		glCompileShader(shaderInfo.vertexShader);
		glCompileShader(shaderInfo.fragmentShader);

		b32 Success;

		Success = ShaderCheckForError(shaderInfo.vertexShader, GL_COMPILE_STATUS, false);
		if (!Success) return EU_INVALID_SHADER_ID;

		Success = ShaderCheckForError(shaderInfo.fragmentShader, GL_COMPILE_STATUS, false);
		if (!Success) return EU_INVALID_SHADER_ID;

		glAttachShader(shaderInfo.program, shaderInfo.vertexShader);
		glAttachShader(shaderInfo.program, shaderInfo.fragmentShader);
		glLinkProgram(shaderInfo.program);

		Success = ShaderCheckForError(shaderInfo.program, GL_LINK_STATUS, true);
		if (!Success) return EU_INVALID_SHADER_ID;

		glDeleteShader(shaderInfo.vertexShader);
		glDeleteShader(shaderInfo.fragmentShader);

		ParseShader(&shaderInfo, ShaderText);

		m_Shaders.Push(shaderInfo);

		String msg = "Compiled and parsed OpenGL shaders (" + path + ")";

		EU_LOG_TRACE(msg.C_Str());

		return m_Shaders.Size();
	}

	void RenderAPIOpenGL::BindShader(ShaderID shader)
	{
		glUseProgram(m_Shaders[shader - 1].program);
		m_BoundShader = shader;
	}

	void RenderAPIOpenGL::SetUniform(const String& name, r32 v, u32 index)
	{
		glUniform1f(m_Shaders[m_BoundShader - 1].uniforms[name].locations[index], v);
	}

	void RenderAPIOpenGL::SetUniform(const String& name, v2 v, u32 index)
	{
		glUniform2f(m_Shaders[m_BoundShader - 1].uniforms[name].locations[index], v.x, v.y);
	}

	void RenderAPIOpenGL::SetUniform(const String& name, v3 v, u32 index)
	{
		glUniform3f(m_Shaders[m_BoundShader - 1].uniforms[name].locations[index], v.x, v.y, v.z);
	}

	void RenderAPIOpenGL::SetUniform(const String& name, v4 v, u32 index)
	{
		glUniform4f(m_Shaders[m_BoundShader - 1].uniforms[name].locations[index], v.x, v.y, v.z, v.w);
	}

	void RenderAPIOpenGL::SetUniform(const String& name, const m4& v, u32 index)
	{
		glUniformMatrix4fv(m_Shaders[m_BoundShader - 1].uniforms[name].locations[index], 1, false, &v[0][0]);
	}

	void RenderAPIOpenGL::BindTextureToShader(const String& name, TextureID texture, u32 index)
	{
		RenderAPIOpenGL* openGL = Engine::GetOpenGL();

		const ShaderSamplerInfo& Sampler = m_Shaders[m_BoundShader - 1].samplers[name];
		BindTexture(texture, Sampler.unit + index);
		glUniform1i(Sampler.locations[index], Sampler.unit + index);
	}

	TextureID RenderAPIOpenGL::AddFramebufferAttachmentTexture(GLuint texture, u32 width, u32 height)
	{
		TextureGL tex;
		tex.id = texture;
		tex.width = width;
		tex.height = height;

		m_Textures.Push(tex);
		return m_Textures.Size();
	}

	GLenum RenderAPIOpenGL::GetTextureFilter(TextureFilter filter)
	{
		switch (filter)
		{
		case EU_TEXTURE_FILTER_LINEAR: return GL_LINEAR;
		case EU_TEXTURE_FILTER_NEAREST: return GL_NEAREST;
		}

		return GL_LINEAR;
	}

	GLenum RenderAPIOpenGL::GetTextureAddressMode(TextureAddressMode addressMode)
	{
		switch (addressMode)
		{
		case EU_TEXTURE_ADDRESS_MODE_REPEAT: return GL_REPEAT;
		case EU_TEXTURE_ADDRESS_MODE_CLAMP_TO_EDGE: return GL_CLAMP_TO_EDGE;
		case EU_TEXTURE_ADDRESS_MODE_CLAMP_TO_BORDER: return GL_CLAMP_TO_BORDER;
		}
	}
}