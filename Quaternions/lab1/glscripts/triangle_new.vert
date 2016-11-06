#version 330 core

in vec3 in_Position;

uniform mat4 ModelMatrix;

uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

void main(void)
{
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(in_Position, 1.0);
}
