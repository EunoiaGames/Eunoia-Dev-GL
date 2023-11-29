#include "Engine.h"
#include "Application.h"
#include "../Rendering/GuiInit.h"
#include "../ECS/ECSLoader.h"

#include "../Utils/Logger.h"
#include "Input.h"

#include "../Platform/OpenGL/RenderAPIOpenGL.h"

namespace Eunoia {

	struct Engine_Data
	{
		Application* app;
		Display* display;
		RenderAPIInterface* renderAPI;
		FinalRenderer* renderer;
		b32 running;
		r32 engineTime;
		b32 renderToSwapchain;
	};

	static Engine_Data s_Engine;

	void Engine::Init(Application* app, const ApplicationInfo& appInfo, RenderAPI renderAPI, b32 renderToSwapchain)
	{
		Logger::Init();
		EUInput::InitInput();
		Metadata::Init();
		ECSLoader::Init();

		s_Engine.app = app;
		s_Engine.running = false;
		s_Engine.display = Display::CreateDisplay();
		s_Engine.display->Init(appInfo.displayInfo);
		s_Engine.renderAPI = RenderAPIInterface::CreateRenderAPI(renderAPI);
		s_Engine.renderAPI->Init(s_Engine.display);
		s_Engine.renderToSwapchain = renderToSwapchain;
		
		s_Engine.renderer = FinalRenderer::CreateFinalRenderer(renderAPI);
		s_Engine.renderer->Init(100000, renderToSwapchain);

		GuiInit::Init();
	}

	void Engine::Destroy()
	{
		delete s_Engine.renderer;
	}

	void Engine::Start()
	{
		s_Engine.running = true;
		s_Engine.app->Init(s_Engine.renderer->GetRenderer2D());

		r32 dt = 1.0f / 144.0f;

		while (s_Engine.running)
		{
			if (s_Engine.display->CheckForEvent(DISPLAY_EVENT_CLOSE))
			{
				Stop();
			}

			Update(dt);
			Render();

			s_Engine.engineTime += dt;
		}
	}

	void Engine::Stop()
	{
		s_Engine.running = false;
	}

	r32 Engine::GetEngineTime()
	{
		return s_Engine.engineTime;
	}

	Display* Engine::GetDisplay()
	{
		return s_Engine.display;
	}

	RenderAPIInterface* Engine::GetRenderAPI()
	{
		return s_Engine.renderAPI;
	}

	RenderAPIOpenGL* Engine::GetOpenGL()
	{
		return (RenderAPIOpenGL*)s_Engine.renderAPI;
	}

	FinalRenderer* Engine::GetRenderer()
	{
		return s_Engine.renderer;
	}

	Renderer2D* Engine::GetRenderer2D()
	{
		return s_Engine.renderer->GetRenderer2D();
	}

	Application* Engine::GetApplication()
	{
		return s_Engine.app;
	}

	v2 Engine::GetOutputSize()
	{
		u32 width, height;
		if (s_Engine.renderToSwapchain)
			s_Engine.display->GetSize(&width, &height);
		else
		{
			TextureID outputTexture = s_Engine.renderer->GetOutputTexture();
			s_Engine.renderAPI->GetTextureSize(outputTexture, &width, &height);
		}

		return v2(width, height);
	}

	void Engine::Update(r32 dt)
	{
		s_Engine.app->PreUpdate(dt);
		s_Engine.display->Update();
		if (s_Engine.app->HasActiveScene())
		{
			s_Engine.app->GetActiveScene()->ecs->Begin();
			s_Engine.app->GetActiveScene()->ecs->UpdateEntities(dt);
		}

		s_Engine.app->Update(dt);
		s_Engine.app->PostUpdate(dt);
	}

	void Engine::Render()
	{
		s_Engine.renderAPI->BeginFrame();
		s_Engine.renderer->BeginFrame();

		GuiInit::Begin();

		s_Engine.app->PreRender(s_Engine.renderer->GetRenderer2D());

		if (s_Engine.app->HasActiveScene())
		{
			s_Engine.app->GetActiveScene()->ecs->RenderEntities();
		}

		s_Engine.app->Render(s_Engine.renderer->GetRenderer2D());

		GuiInit::End();

		s_Engine.app->PostRender(s_Engine.renderer->GetRenderer2D());

		s_Engine.renderer->EndFrame();
		s_Engine.renderer->RenderFrame();
		GuiInit::Render();

		s_Engine.renderAPI->EndFrame();

		s_Engine.display->Present();
	}

}
