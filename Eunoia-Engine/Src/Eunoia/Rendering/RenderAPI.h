#pragma once

#include "../Common.h"
#include "../DataStructures/String.h"
#include "RenderObjectIDs.h"

namespace Eunoia {

	enum RenderAPI
	{
		EU_RENDER_API_OPENGL
	};

	enum TextureFilter
	{
		EU_TEXTURE_FILTER_NEAREST,
		EU_TEXTURE_FILTER_LINEAR,
	};

	enum TextureAddressMode
	{
		EU_TEXTURE_ADDRESS_MODE_REPEAT,
		EU_TEXTURE_ADDRESS_MODE_CLAMP_TO_EDGE,
		EU_TEXTURE_ADDRESS_MODE_CLAMP_TO_BORDER
	};

	class Display;
	class RenderAPIInterface
	{
	public:
		virtual void Init(Display* display) = 0;
		virtual void Destroy() = 0;

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;

		virtual TextureID CreateTexture(const u8* pixels, u32 width, u32 height, TextureFilter filter = EU_TEXTURE_FILTER_LINEAR,
			TextureAddressMode addressMode = EU_TEXTURE_ADDRESS_MODE_REPEAT) = 0;

		virtual TextureID CreateTexture(const String& path, TextureFilter filter = EU_TEXTURE_FILTER_LINEAR,
			TextureAddressMode addressMode = EU_TEXTURE_ADDRESS_MODE_REPEAT) = 0;

		virtual void DestroyTexture(TextureID texture) = 0;

		virtual void GetTextureSize(TextureID texture, u32* width, u32* height) = 0;

		virtual ShaderID CreateShader(const String& path) = 0;
	public:
	public:
		static RenderAPIInterface* CreateRenderAPI(RenderAPI api);
	};
}