#EU_Vertex
#version 330

layout(location = 0) in vec2 Pos;

out vec2 TexCoord0;

void main()
{
	gl_Position = vec4(Pos, 0.0, 1.0);
	TexCoord0 = Pos * 0.5 + 0.5;
}

#EU_Fragment
#version 330

in vec2 TexCoord0;

uniform sampler2D Output2D;

out vec4 OutColor;

void main()
{
	OutColor = texture(Output2D, TexCoord0);
}