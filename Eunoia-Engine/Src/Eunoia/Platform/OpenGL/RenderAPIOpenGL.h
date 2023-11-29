#pragma once

#include "../../Rendering/RenderAPI.h"
#include "../../DataStructures/String.h"
#include "../../DataStructures/Map.h"

#include <GL\glew.h>
#ifdef EU_PLATFORM_WINDOWS
#include <GL\wglew.h>
#include "../Windows/DisplayWin32.h"
#endif

namespace Eunoia {

	typedef u32 TextureID;
	typedef u32 TextureUnit;

	struct TextureGL
	{
		GLuint id;
		u32 width;
		u32 height;
		String path;
	};

	struct ShaderSamplerInfo
	{
		TextureUnit unit;
		List<u32> locations;
		u32 arrayLength;
	};

	struct ShaderUniformInfo
	{
		List<u32> locations;
		u32 arrayLength;
	};

	struct ShaderInfo
	{
		GLuint program;
		GLuint vertexShader;
		GLuint fragmentShader;
		Map<String, ShaderUniformInfo> uniforms;
		Map<String, ShaderSamplerInfo> samplers;
	};

	class EU_API RenderAPIOpenGL : public RenderAPIInterface
	{
	public:
		virtual void Init(Display* display) override;
		virtual void Destroy() override;

		virtual void BeginFrame();
		virtual void EndFrame();

		virtual TextureID CreateTexture(const u8* pixels, u32 width, u32 height, TextureFilter filter = EU_TEXTURE_FILTER_LINEAR, 
			TextureAddressMode addressMode = EU_TEXTURE_ADDRESS_MODE_REPEAT) override;

		virtual TextureID CreateTexture(const String& path, TextureFilter filter = EU_TEXTURE_FILTER_LINEAR,
			TextureAddressMode addressMode = EU_TEXTURE_ADDRESS_MODE_REPEAT) override;

		TextureGL* GetTextureGL(TextureID texture);

		virtual void GetTextureSize(TextureID texture, u32* width, u32* height);

		virtual void DestroyTexture(TextureID texture) override;

		void BindTexture(TextureID texture, TextureUnit unit);
		GLuint GetTextureHandleGL(TextureID texture);

		virtual ShaderID CreateShader(const String& path) override;
		void BindShader(ShaderID shader);
		void SetUniform(const String& name, r32 v, u32 index = 0);
		void SetUniform(const String& name, v2 v, u32 index = 0);
		void SetUniform(const String& name, v3 v, u32 index = 0);
		void SetUniform(const String& name, v4 v, u32 index = 0);
		void SetUniform(const String& name, const m4& v, u32 index = 0);
		void BindTextureToShader(const String& name, TextureID texture, u32 index = 0);

		TextureID AddFramebufferAttachmentTexture(GLuint texture, u32 width, u32 height);


	public:
		static GLenum GetTextureFilter(TextureFilter filter);
		static GLenum GetTextureAddressMode(TextureAddressMode addressMode);
	private:
		HGLRC m_GLRC;
		List<TextureGL> m_Textures;
		List<TextureID> m_FreeTextureIDs;
		List<ShaderInfo> m_Shaders;
		ShaderID m_BoundShader;
	};

}
