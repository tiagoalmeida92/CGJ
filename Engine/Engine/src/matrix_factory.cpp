#include "matrix_factory.hpp"

Mat3 identity3() {
	return Mat3{
		1, 0, 0,
		0, 1, 0,
		0, 0, 1
	};
}

Mat4 identity4() {
	return Mat4 {
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
	return Mat4 {
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
