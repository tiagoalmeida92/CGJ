#include "GLGameObject.hpp"

const GLfloat Colors[]{
	0.0f, 0.5f, 0.5f, 1.0f,
	0.75f, 0.0f, 0.5f, 1.0f,
	0.5f, 0.5f, 1.0f, 1.0f,
	0.75f, 0.5f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f,
	0.5f, 0.0f, 0.5f, 1.0f,
	0.75f, 0.0f, 0.5f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f,
	0.5f, 0.0f, 0.5f, 1.0f,
};


GLGameObject::GLGameObject(vector<int> faceColors, int start, vector<int> offsets, int uniformId, int uniformColorId, Mat4 modelMatrix)
{
	this->faceColors = faceColors;
	this->startIdx = start;
	this->offsets = offsets;
	this->uniformId = uniformId;
	this->uniformColorId = uniformColorId;
	this->modelMatrix = modelMatrix;
}

void GLGameObject::draw() {
	int currentIdx = startIdx;

	
	glUniformMatrix4fv(uniformId, 1, GL_FALSE, (modelMatrix).convert_opengl());
	for (int i = 0; i < faceColors.size(); ++i) {
		int offset = offsets[i];
		glUniform4fv(uniformColorId, 1, &Colors[i*4]);
		glDrawElements(GL_TRIANGLES, offset, GL_UNSIGNED_BYTE, (void*)currentIdx);
		currentIdx += offset;
	}
}