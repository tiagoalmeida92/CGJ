#pragma once
#include "mat.hpp"


	Mat3 identity3();


	Mat3 dual(Vec3&);

	Mat3 scaling3(Vec3&);
	Mat3 rotate3(Vec3&, float);
	
	
	Mat4 identity4();
	Mat4 scaling4(Vec3&);
	Mat4 translate(Vec3&);


	Mat4 rotate4(Vec3&, float);

	Mat4 convertTo4(Mat3&);
	Mat3 convertTo3(Mat4&);
	
	Mat4 lookAt(Vec3&, Vec3&, Vec3&);

	Mat4 ortho(float l, float r, float b, float t, float n, float f);
	Mat4 perspective(float fovy, float aspect, float zNear, float zFar);