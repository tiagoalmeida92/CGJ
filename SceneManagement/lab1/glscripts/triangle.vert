#version 330 core

in vec3 in_Position;
in vec3 inTexcoord;
in vec3 inNormal;

out vec3 ex_Normal;

uniform mat4 Matrix;
uniform mat4 Camera;

void main(void)
{
		gl_Position = Camera * Matrix * vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);
		ex_Normal = inNormal;
}