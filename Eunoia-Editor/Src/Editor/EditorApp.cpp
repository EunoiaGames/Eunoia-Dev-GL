#include "EditorApp.h"
#include "EditorGUI.h"
#include "ProjectManager.h"


void EditorApp::Init(Renderer2D* renderer)
{	
	CreateScene("Main");

	EditorGUI::Init();
	ProjectManager::Init();
}

void EditorApp::Update(r32 dt)
{
	EunoiaProject* project = ProjectManager::GetProject();
	
	if (project->loaded)
	{
		if (project->stepApplication)
		{
			if (project->application->HasActiveScene())
			{
				project->application->GetActiveScene()->ecs->Begin();
				project->application->GetActiveScene()->ecs->UpdateEntities(dt);
				project->application->Update(dt);
			}
		}
		else
		{
			if (project->application->HasActiveScene())
			{
				project->application->GetActiveScene()->ecs->UpdateTransformHierarchySystems(dt);
			}
		}
	}
}

void EditorApp::Render(Renderer2D* renderer)
{
	EunoiaProject* project = ProjectManager::GetProject();

	if (project->loaded)
	{
		if (project->application->HasActiveScene())
		{
			project->application->GetActiveScene()->ecs->RenderEntities();
			project->application->Render(renderer);
		}
	}
}

void EditorApp::PostRender(Renderer2D* renderer)
{
	EditorGUI::Begin();
	EditorGUI::DrawGUI();
	EditorGUI::End();
	EditorGUI::DrawFrame();
}

int main()
{
	ApplicationInfo info;
	info.name = "Eunoia Editor";
	info.version = { 1, 0, 0 };
	info.displayInfo.title = "Eunoia Editor";
	info.displayInfo.res = EU_DISPLAY_RESOLUTION_16_9;
	info.displayInfo.resScale = 80;
	Engine::Init(new EditorApp(), info, EU_RENDER_API_OPENGL, false);
	Engine::Start();

	return 0;
}
