#include "GLGameObject.hpp"

const GLfloat Colors[]{
	1, 0, 0,
	0.73, 0.25, 0.25,
	0.36, 0.12, 0.12,
	0.86, 0.58, 0.58,
	0.36, 0.02, 0.02,
	0.73, 0.09, 0.09,

	0, 1, 0,
	0.14, 0.86, 0.14,
	0.54, 0.93, 0.54,
	0.05, 0.35, 0.05,
	0.17, 0.23, 0.17,
	0.00, 0.20, 0.00
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
		int colorIdx = faceColors[i];
		glUniform3fv(uniformColorId, 1, &Colors[colorIdx*3]);
		glDrawElements(GL_TRIANGLES, offset, GL_UNSIGNED_BYTE, (void*)currentIdx);
		currentIdx += offset;
	}
}






//const GLfloat Colors[]{
//	0.0f, 0.5f, 0.5f, 1.0f,
//	0.75f, 0.0f, 0.5f, 1.0f,
//	0.5f, 0.5f, 1.0f, 1.0f,
//	0.75f, 0.5f, 0.0f, 1.0f,
//	1.0f, 1.0f, 0.0f, 1.0f,
//	0.0f, 1.0f, 1.0f, 1.0f,
//	0.5f, 0.0f, 0.5f, 1.0f,
//	0.75f, 0.0f, 0.5f, 1.0f,
//	1.0f, 1.0f, 0.0f, 1.0f,
//	0.0f, 1.0f, 1.0f, 1.0f,
//	0.5f, 0.0f, 0.5f, 1.0f,
//};
