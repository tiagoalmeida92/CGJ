#include "gl_shader.hpp"


string read(char * path) {
	string line;
	string result;
	ifstream myfile(path);

	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			result.append(line + "\n");
		}
		myfile.close();
	}

	return result;


}

void logShaderError(GLint shaderId) {
	GLint maxLength = 0;
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

	std::vector<GLchar> errorLog(maxLength);
	glGetShaderInfoLog(shaderId, maxLength, &maxLength, &errorLog[0]);

	for (size_t i = 0; i < maxLength; i++)
	{
		char c = errorLog[i];
		if (c == NULL) {
			break;
		}
		cout << c;
	}

	glDeleteShader(shaderId); // Don't leak the shader.
}

Shader CreateProgram(char* vertexShaderFile, char* fragmentShaderFile) {
	string vertexShader = read(vertexShaderFile);
	string fragShader = read(fragmentShaderFile);
	const char* vertexShaderPtr = vertexShader.c_str();
	const char* fragShaderPtr = fragShader.c_str();
	int vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderId, 1, &vertexShaderPtr, 0);
	glCompileShader(vertexShaderId);

	Shader shader;
	shader.compiled = false;
	GLint isCompiled = 0;
	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		logShaderError(vertexShaderId);
		return shader;
	}

	int fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, &fragShaderPtr, 0);
	glCompileShader(fragmentShaderId);
	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		logShaderError(vertexShaderId);
		return shader;
	}

	int programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);
	
	shader.compiled = true;
	shader.ProgramId = programId;
	shader.VertexShaderId = vertexShaderId;
	shader.FragmentShaderId = fragmentShaderId;

	return shader;
}

void Shader::BindAttributeLocation(GLubyte idx, const char* name) {
	glBindAttribLocation(ProgramId, idx, name);
}

GLint GetUniformLocation(GLint pId, const char* name) {
	return glGetUniformLocation(pId, name);
}

GLint Shader::addUniform(string name) {
	GLint location = GetUniformLocation(ProgramId, name.c_str());
	uniforms[name] = location;
	return location;
}

void Shader::destroyShader() {
	glUseProgram(0);
	glDetachShader(ProgramId, VertexShaderId);
	glDetachShader(ProgramId, FragmentShaderId);

	glDeleteShader(FragmentShaderId);
	glDeleteShader(VertexShaderId);
	glDeleteProgram(ProgramId);
}