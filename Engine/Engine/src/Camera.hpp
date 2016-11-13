#pragma once

#include "GL\glew.h"

class Camera {
	public:
	
		Camera(GLint cameraUid);

		void setViewMatrix();
		void setProjectionMatrix();

	private:
		GLint CameraUId;
};