#include "Meep.h"

void Meep::Init(Renderer2D* renderer2D)
{

}

void Meep::OnClose()
{

}

Application* CreateApplication(ApplicationInfo* appInfo)
{
	appInfo->version = {1, 0, 0};

	appInfo->name = "Meep";
	appInfo->displayInfo.title = "Meep";
	appInfo->displayInfo.res = EU_DISPLAY_RESOLUTION_16_9;
	appInfo->displayInfo.resScale = 80;

	return new Meep();
}