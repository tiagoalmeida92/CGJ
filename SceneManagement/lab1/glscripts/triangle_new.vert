#version 330 core

in vec3 inPosition;
in vec3 inNormal;

out vec3 ex_Normal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform	mat4 ProjectionMatrix;

void main(void)
{
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(inPosition, 1.0);
	ex_Normal = inNormal;
}
