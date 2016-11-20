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


	void BindAttributeLocation(GLubyte, const char*);

	//GLint GetUniformLocation(GLint, const char*);

	void destroyShader();

	GLint addUniform(string name);

} Shader;

Shader CreateProgram(char*, char*);

