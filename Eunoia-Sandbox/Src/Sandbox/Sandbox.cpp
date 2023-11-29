#include "Sandbox.h"


#define TILE_SIZE 100
#define TILE_GAP 10

void Sandbox::Init(Renderer2D* renderer2D)
{
	CreateScene("Main");
	
	EntityID quad = GetECS()->CreateEntity("Quad");
	GetECS()->CreateComponent<Transform2DComponent>(quad);
	GetECS()->CreateComponent<SpriteComponent>(quad, SpriteComponent(v2(50, 50), v4(0.0f, 0.0f, 1.0f, 1.0f)));

	GetECS()->CreateSystem<SpriteSubmissionSystem>();
	GetECS()->CreateSystem<TransformHierarchy2DSystem>();

	EU_LOG_INFO("Initialized Application");
}


void Sandbox::Render(Renderer2D* renderer2D)
{
	static char Test[512];
	if(ImGui::Begin("Hello"))
	{
		ImGui::InputText("Input", Test, 512);
	}
	ImGui::End();
}

void Sandbox::Update(r32 dt)
{
	m_Time += dt;
}

int main()
{
	ApplicationInfo appInfo;
	appInfo.displayInfo.title = "Eunoia Engine";
	appInfo.displayInfo.res = EU_DISPLAY_RESOLUTION_16_9;
	appInfo.displayInfo.resScale = 85;
	appInfo.name = "EunoiaEngine";
	appInfo.version = { 1, 0, 0 };
	Engine::Init(new Sandbox(), appInfo, EU_RENDER_API_OPENGL, true);
	Engine::Start();

	Engine::Destroy();
}