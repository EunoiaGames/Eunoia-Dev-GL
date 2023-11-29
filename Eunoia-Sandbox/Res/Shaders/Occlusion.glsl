#EU_Vertex
#version 330

layout(location = 0) in vec2 Pos;

uniform mat4 Projection;
uniform vec2 Translation;

void main()
{
	gl_Position = Projection * vec4(Pos + Translation, 0.0, 1.0);
}

#EU_Fragment
#version 330

out vec4 OutColor;

void main()
{
	OutColor = vec4(1.0, 1.0, 1.0, 1.0);
}
