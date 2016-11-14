#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

#include "GL/glew.h"
#include "GL/freeglut.h"


using namespace std;

typedef struct {
	bool compiled;
	GLint FragmentShaderId;
	GLint VertexShaderId;
	GLint ProgramId;
	std::map<string, GLint> uniforms;


} Shader;

Shader CreateProgram(char*, char*);

void BindAttributeLocation(GLint, GLubyte, char*);

GLint GetUniformLocation(GLint, char*);

void DestroyShader(Shader);

//TODO
void addUniform(Shader& shader, string name);
