#pragma once

#include "vec.hpp"
#include "mat.hpp"

struct qtrn
{
	float t, x, y, z;

	qtrn();

	qtrn(float t, float x, float y, float z);
	

	const friend qtrn fromAngleAxis(float theta, Vec4& axis);

	const void toAngleAxis(float& theta, Vec4& axis);

	const void clean();
	const float quadrance();
	const float norm();
	const qtrn normalize();
	const qtrn conjugate();
	const qtrn inverse();

	const friend bool operator == (const qtrn& q0, const qtrn& q1);
	const friend qtrn operator + (qtrn& q0, qtrn& q1);
	const friend qtrn operator *(const qtrn& q, const float s);
	const friend qtrn operator *(const qtrn& q0, const qtrn& q1);


	const Mat4 toMatrix();

	const friend qtrn lerp(const qtrn& q0, const qtrn& q1, float k);
	const friend qtrn slerp(const qtrn& q0, const qtrn& q1, float k);

	friend ostream& operator<< (ostream &out, const qtrn &v);
	
};

const float qThreshold = (float)1.0e-5;

