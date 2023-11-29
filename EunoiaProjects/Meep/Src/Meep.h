#pragma once
#include <Eunoia\Eunoia.h>

using namespace Eunoia;

class EU_PROJ Meep : public Application
{
public:
	void Init(Renderer2D* renderer2D) override;
	void OnClose() override;
	void RegisterMetadata() override;

};

extern "C" EU_PROJ Application* CreateApplication(ApplicationInfo* appInfo);