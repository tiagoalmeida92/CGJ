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

	if (mesh_ && shader_) {
		glUniformMatrix4fv(shader_->uniforms["Matrix"], 1, GL_FALSE, (model_matrix).convert_opengl());
		mesh_->draw();
	}
	for (size_t i = 0; i < children.size(); i++)
	{
		if (!children[i]->shader_) {
			children[i]->shader_ = shader_;
		}
		children[i]->draw();
	}
}


SceneGraph::SceneGraph() {
	root = SceneNode();
}

void SceneGraph::setCamera(Camera* camera) {
	camera_ = camera;
}

Camera * SceneGraph::getCamera() {
	return camera_;
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
