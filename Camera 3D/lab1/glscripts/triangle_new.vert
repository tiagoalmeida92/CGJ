#version 330 core

in vec3 in_Position;
in vec4 in_Color;
out vec4 ex_Color;

uniform mat4 ModelMatrix;

uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

void main(void)
{
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);
	ex_Color = in_Color;
}
