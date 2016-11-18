#include "Camera.hpp"

Camera::Camera(GLint cameraUid) {
	CameraUId = cameraUid;
}

void Camera::setViewMatrix(Mat4& viewMatrix) {
	view_matrix = viewMatrix;
	
}
void Camera::setProjectionMatrix(Mat4& projectionMatrix) {
	projection_matrix = projectionMatrix;
}

Mat4 Camera::get_matrix() {
	return projection_matrix * view_matrix;
}