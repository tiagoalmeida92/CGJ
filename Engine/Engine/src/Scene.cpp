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

void SceneNode::draw(Mat4& parents_matrix) {

	if (mesh_ && shader_) {
		glUseProgram(shader_->ProgramId);
		glUniformMatrix4fv(shader_->uniforms["Matrix"], 1, GL_FALSE, (parents_matrix * model_matrix).convert_opengl());
		mesh_->draw();
		glUseProgram(0);
	}
	for (size_t i = 0; i < children.size(); i++)
	{
		if (!children[i]->shader_) {
			children[i]->shader_ = shader_;
		}
		children[i]->draw(parents_matrix * model_matrix);
	}
}

void SceneNode::destroy() {
	for (size_t i = 0; i < children.size(); i++)
	{
		children[i]->destroy();
	}
	children.clear();
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
	root.draw(identity4());
}

void SceneGraph::destroy() {
	root.destroy();
}