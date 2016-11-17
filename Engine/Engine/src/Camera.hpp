#pragma once

#include "GL\glew.h"
#include "mat.hpp"

class Camera {
	public:
	
		Camera(GLint cameraUid);

		void setViewMatrix(Mat4& viewMatrix);
		void setProjectionMatrix(Mat4& projectionMatrix);

		GLint CameraUId;

	private:


		Mat4 view_matrix;
		Mat4 projection_matrix;
};