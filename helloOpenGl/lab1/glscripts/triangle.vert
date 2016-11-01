#version 330 core

in vec2 in_Position;

uniform mat4 Matrix;

void main(void)
{
		gl_Position = Matrix * vec4(in_Position.x, in_Position.y, 0.0, 1.0);
}