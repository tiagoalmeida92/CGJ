#version 330 core

out vec4 out_Color;

uniform vec3 Color;

void main(void)
{
	out_Color = vec4(Color,1);
}