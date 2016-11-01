#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "GL/glew.h"
#include "GL/freeglut.h"


using namespace std;

typedef struct {
	bool compiled;
	GLint FragmentShaderId;
	GLint VertexShaderId;
	GLint ProgramId;
} Shader;

Shader CreateProgram(char*, char*);

void BindAttributeLocation(GLint, GLubyte, char*);

GLint GetUniformLocation(GLint, char*);

void DestroyShader(Shader);