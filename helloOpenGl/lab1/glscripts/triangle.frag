#version 330 core

out vec4 out_Color;

uniform vec4 Color;

void main(void)
{
	out_Color = Color;
}