#EU_Vertex
#version 330

layout(location = 0) in vec2 Pos;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec4 Color;
layout(location = 3) in float TexIndex;

out vec4 Color0;
out vec2 TexCoord0;
flat out float TexIndex0;

uniform mat4 ViewProjection;

void main()
{
	gl_Position = ViewProjection * vec4(Pos, 0.0, 1.0);
	TexCoord0 = TexCoord;
	Color0 = Color;
	TexIndex0 = TexIndex;
}

#EU_Fragment
#version 330

in vec4 Color0;
in vec2 TexCoord0;
flat in float TexIndex0;

layout(location = 0) out vec4 OutColor;

uniform vec4 Ambient;

uniform sampler2D Textures[32];

void main()
{
	int Index = int(floor(TexIndex0 + 0.01));
	vec4 texSample = texture(Textures[Index], TexCoord0);
	OutColor = Color0 * texSample * Ambient;
}