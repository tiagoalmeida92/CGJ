#pragma once
#include "mat.hpp"

class MatrixFactory {
public:
	static Mat3 identity3();


	static Mat3 dual(Vec3&);

	static Mat3 scaling3(Vec3&);
	static Mat3 rotate3(Vec3&, float);
	
	
	static Mat4 identity4();
	static Mat4 scaling4(Vec3&);
	static Mat4 translate(Vec3&);


	static Mat4 rotate4(Vec3&, float);

	static Mat4 convertTo4(Mat3&);
	static Mat3 convertTo3(Mat4&);
	
};