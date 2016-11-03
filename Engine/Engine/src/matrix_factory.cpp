#include "matrix_factory.hpp"

Mat3 identity3() {
	return Mat3{
		1, 0, 0,
		0, 1, 0,
		0, 0, 1
	};
}

Mat4 identity4() {
	return Mat4{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
}

Mat3 scaling3(Vec3& factors) {
	return Mat3{
		factors.x, 0.0f, 0.0f,
		0.0f, factors.y, 0.0f,
		0.0f, 0.0f, factors.z
	};
}

Mat4 scaling4(Vec3& factors) {
	return convertTo4(scaling3(factors));
}

Mat4 translate(Vec3& vec) {
	return Mat4{
		1, 0, 0, vec.x,
		0, 1, 0, vec.y,
		0, 0, 1, vec.z,
		0, 0, 0, 1
	};
}

Mat3 dual(Vec3& axis) {
	return Mat3{
			0, -axis.z, axis.y,
			axis.z, 0, -axis.x,
			-axis.y, axis.x, 0
	};
}

//Rodrigues formula
Mat3 rotate3(Vec3& axis, float angle) {
	Mat3 a = dual(axis);
	Mat3 rodrigues = identity3() + sin(angle) * a + (1 - cos(angle)) * (a * a);
	return rodrigues;

}

Mat4 rotate4(Vec3& axis, float angle) {
	Mat3 result = rotate3(axis, angle);
	return convertTo4(result);

}


Mat4 convertTo4(Mat3& mat3) {
	return Mat4{
		mat3.data[0], mat3.data[1], mat3.data[2],   0,
		mat3.data[3], mat3.data[4], mat3.data[5],	0,
		mat3.data[6], mat3.data[7], mat3.data[8],	0,
		0,				0,			0,			1
	};
}

Mat3 convertTo3(Mat4& mat4) {
	return Mat3{
		mat4.data[0], mat4.data[1], mat4.data[2],
		mat4.data[4], mat4.data[5], mat4.data[6],
		mat4.data[8], mat4.data[9], mat4.data[10]
	};
}

Mat4 lookAt(Vec3& eye, Vec3 &center, Vec3 &up) {
	Mat4 result;

	Vec3 s, u, v;

	v = eye - center;
	v = v.normalize();
	u = up;
	s = u.cross(v);


	u = v.cross(s);

	s = s.normalize();
	u = u.normalize();

	result.data[0] = s.x;
	result.data[1] = s.y;
	result.data[2] = s.z;
	result.data[3] = -s.dot(eye);
	result.data[4] = u.x;
	result.data[5] = u.y;
	result.data[6] = u.z;
	result.data[7] = -u.dot(eye);
	result.data[8] = v.x;
	result.data[9] = v.y;
	result.data[10] = v.z;
	result.data[11] = -v.dot(eye);
	result.data[12] = 0;
	result.data[13] = 0;
	result.data[14] = 0;
	result.data[15] = 1.0f;
	return result;
}

Mat4 ortho(float l, float r, float b, float t, float n, float f) {

	Mat4 result;
	result.data[0] = 2 / (r-l);
	result.data[1] = 0;
	result.data[2] = 0;
	result.data[3] = (l+r)/(l-r);

	result.data[4] = 0;
	result.data[5] = 2/(t-b);
	result.data[6] = 0;
	result.data[7] = (b+t)/(b-t);

	result.data[8] = 0;
	result.data[9] = 0;
	result.data[10] = 2/(n-f);
	result.data[11] = (n+f)/(n-f);

	result.data[12] = 0;
	result.data[13] = 0;
	result.data[14] = 0;
	result.data[15] = 1;

	return result;
}

#define PI 3.14159265358979f
#define TO_RAD(f) (f * (PI / 180.0f))

Mat4 perspective(float fovy, float aspect, float zNear, float zFar) {
	float fovyRad = TO_RAD(fovy);
	float theta = fovyRad / 2;
	float d = 1 / (tan(theta));
	Mat4 result;
	result.data[0] = d / aspect;
	result.data[1] = 0;
	result.data[2] = 0;
	result.data[3] = 0;

	result.data[4] = 0;
	result.data[5] = d;
	result.data[6] = 0;
	result.data[7] = 0;

	result.data[8] = 0;
	result.data[9] = 0;
	result.data[10] = (zFar + zNear) / (zNear - zFar);
	result.data[11] = (2* zFar*zNear) / (zNear - zFar);

	result.data[12] = 0;
	result.data[13] = 0;
	result.data[14] = -1;
	result.data[15] = 0;

	return result;
}