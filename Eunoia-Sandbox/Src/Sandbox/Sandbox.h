#pragma once

#include <Eunoia\Eunoia.h>

using namespace Eunoia;

struct Sprite
{
	v2 pos;
	v2 size;
	v4 color;
	TextureID texture;
	b32 occluder;
};

class Sandbox : public Application
{
public:
	virtual void Init(Renderer2D* renderer2D) override;
	virtual void Render(Renderer2D* renderer2D) override;
	virtual void Update(r32 dt) override;
private:
	List<Sprite> m_Sprites;
	r32 m_Time;
	TextureID m_GraniteTexture;
};