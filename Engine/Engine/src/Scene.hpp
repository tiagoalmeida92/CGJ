#pragma once
#include "Camera.hpp"
#include "Mesh.hpp"
#include "mat.hpp"
#include "gl_shader.hpp"
#include <vector>
#include "GL/glew.h"
#include "matrix_factory.hpp"

class SceneNode {
public:
	SceneNode();

	void setMesh(Mesh* mesh);
	void setMatrix(Mat4& matrix);
	SceneNode* createNode();
	void setShaderProgram(Shader* shader);
	void draw();


private:
	vector<SceneNode*> children;
	Mat4 model_matrix;
	Mesh* mesh_;
	Shader* shader_;
};

class SceneGraph {
public:
	SceneGraph();

	void setCamera(Camera* camera);
	SceneNode* getRoot();
	SceneNode* createNode();
	void draw();

private:
	SceneNode root;
	Camera* camera_;


};