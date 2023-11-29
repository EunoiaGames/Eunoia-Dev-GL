#pragma once

#include <Eunoia\Eunoia.h>

using namespace Eunoia;


class EditorApp : public Application
{
public:
	virtual void Init(Renderer2D* renderer) override;
	virtual void Update(r32 dt) override;
	virtual void Render(Renderer2D* renderer) override;
	virtual void PostRender(Renderer2D* renderer) override;
};

