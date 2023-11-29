#pragma once

#include "ApplicationInfo.h"
#include "../Rendering/Display.h"
#include "../Rendering/RenderAPI.h"
#include "../Rendering/FinalRenderer.h"

namespace Eunoia {

	class Application;
	class RenderAPIOpenGL;
	class Renderer2D;
	class EU_API Engine
	{
	public:
		static void Init(Application* app, const ApplicationInfo& appInfo, RenderAPI renderAPI, b32 renderToSwapchain = true);
		static void Destroy();
		static void Start();
		static void Stop();

		static r32 GetEngineTime();

		static Display* GetDisplay();
		static RenderAPIInterface* GetRenderAPI();
		static RenderAPIOpenGL* GetOpenGL();
		static FinalRenderer* GetRenderer();
		static Renderer2D* GetRenderer2D();
		static Application* GetApplication();
		static v2 GetOutputSize();
	private:
		static void Update(r32 dt);
		static void Render();
	};

}
