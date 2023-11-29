#pragma once

#include "RenderAPI.h"
#include "Renderer2D.h"

namespace Eunoia {

	class FinalRenderer
	{
	public:
		virtual void Init(u32 maxSprites, b32 renderToSwapchain = true) = 0;
		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;
		virtual void RenderFrame() = 0;

		virtual void ResizeOutput(u32 width, u32 height) = 0;

		virtual Renderer2D* GetRenderer2D() = 0;

		virtual TextureID GetOutputTexture() = 0;
	public:
		static FinalRenderer* CreateFinalRenderer(RenderAPI renderAPI);
	};

}