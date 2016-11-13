#include "Scene.hpp"

SceneNode::SceneNode() {
}

void SceneNode::setMesh(Mesh* mesh) {
	mesh_ = mesh;
}

void SceneNode::setMatrix(Mat4& matrix) {
	model_matrix = matrix;
}

SceneNode* SceneNode::createNode() {
	SceneNode* node = new SceneNode();
	children.push_back(node);
	return node;
}

void SceneNode::setShaderProgram(Shader* shader) {
	shader_ = shader;
}

void SceneNode::draw() {
	for (size_t i = 0; i < children.size(); i++)
	{
		children[i]->draw();
	}
	if (mesh_) {
		
		//glUniformMatrix4fv(, 1, GL_FALSE, identity4().convert_opengl());

		//glDrawArrays(GL_TRIANGLES, 0, 24);
	}
}


SceneGraph::SceneGraph() {
	root = SceneNode();
}

void SceneGraph::setCamera(Camera* camera) {
	camera_ = camera;
}

SceneNode* SceneGraph::getRoot() {
	return &root;
}

SceneNode* SceneGraph::createNode() {
	return root.createNode();
}


void SceneGraph::draw() {
	root.draw();
}
