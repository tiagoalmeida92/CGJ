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


const GLfloat Colors[]{

	0, 1, 0,
	0.14, 0.86, 0.14,
	0.54, 0.93, 0.54,
	0.05, 0.35, 0.05,
	0.17, 0.23, 0.17,
	0.00, 0.20, 0.00,

	1, 0, 0,
	0.73, 0.25, 0.25,
	0.36, 0.12, 0.12,
	0.86, 0.58, 0.58,
	0.36, 0.02, 0.02,
	0.73, 0.09, 0.09,

	0, 0, 1,
	0.15, 0.15, 0.85,
	0.07, 0.07, 0.37,
	0.64, 0.64, 0.93,
	0.02, 0.02, 0.20,
	//17
	1, 0.6, 0,
	0.7176471,0.4627451,0.08235294,
	0.8,0.5686275,0.21960784,
	0.9529412,0.7490196,0.44705883,
	0.7176471,0.4392157,0.023529412,

	0.81,0,1,
	0.6666667,0.1254902,0.77254903,
	0.7490196,0.32941177,0.83137256,
	0.74509805,0.43137255,0.8117647,
	0.6784314,0.15686275,0.78431374,

	0,0.65,1,
	0.2784314,0.6,0.7607843,
	0.13333334,0.60784316,0.84705883,
	0.47843137,0.8,0.9607843,
	0.4,0.78431374,0.9764706,

	0,1,0.235,
	0.38039216,0.8784314,0.49803922,
	0.08235294,0.81960785,0.25490198,
	0.23921569,0.98039216,0.4117647,
	0.047058824,0.9137255,0.2509804


};