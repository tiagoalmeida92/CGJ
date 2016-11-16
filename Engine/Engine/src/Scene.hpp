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
	void draw(Mat4&);


private:
	vector<SceneNode*> children;
	Mat4 model_matrix = identity4();
	Mesh* mesh_ = nullptr;
	Shader* shader_ = nullptr;
};

class SceneGraph {
public:
	SceneGraph();

	void setCamera(Camera* camera);
	Camera * getCamera();
	SceneNode* getRoot();
	SceneNode* createNode();
	void draw();

private:
	SceneNode root;
	Camera* camera_;


};