#version 330 core

in vec3 in_Position;

uniform mat4 Matrix;

void main(void)
{
		gl_Position = Matrix * vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);
}