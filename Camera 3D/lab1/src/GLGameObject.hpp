#pragma once

#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "vec.hpp"
#include "mat.hpp"

using namespace std;

class GLGameObject
{
public:
	GLGameObject(vector<int> faceColors, int startIdx, vector<int> offsets, int uniformId, int uniformColorId, Mat4 modelMatrix);
	void GLGameObject::draw();

	vector<int> faceColors;
	int startIdx;
	vector<int> offsets;

	int uniformId;
	int uniformColorId;
	Mat4 modelMatrix;

};

