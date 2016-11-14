#include "Camera.hpp"

Camera::Camera(GLint cameraUid) {
	CameraUId = cameraUid;
}

void Camera::setViewMatrix(Mat4& viewMatrix) {
	view_matrix = viewMatrix;
	glUniformMatrix4fv(CameraUId, 1, GL_FALSE, (projection_matrix * view_matrix).convert_opengl());
}
void Camera::setProjectionMatrix(Mat4& projectionMatrix) {
	projection_matrix = projectionMatrix;
}