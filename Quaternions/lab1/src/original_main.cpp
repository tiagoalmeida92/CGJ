///////////////////////////////////////////////////////////////////////
//
// Using quaternions to rotate in 3D.
//
// Assignment: 1. Create a class for Quaternions.
//             2. Create a scene with a camera rotating around an 
//                object at the origin and pointing towards it. 
//                Do NOT use "gluLookAt", use rotation matrices!
//             3. Gimbal lock mode ON: use Translation + Rotation 
//                matrices (e.g. Euler angles, Rodrigues’ formula). 
//             4. Gimbal lock mode OFF: use Quaternions to produce a 
//                transformation matrix to avoid gimbal lock.
//             5. Switch between modes by pressing the 'g' key.
//
// (c) 2013-16 by Carlos Martinho
//
///////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <string>
#include <cassert>

#define DEGREES_TO_RADIANS 0.01745329251994329547
#define RADIANS_TO_DEGREES 57.29577951308232185913

/////////////////////////////////////////////////////////////////////// VECTOR

typedef struct {
	float x, y, z, w;
} vec4;

const float vThreshold = (float)1.0e-5;

const vec4 vNormalize(const vec4& v)
{
	vec4 vn;
	float s = 1 / (v.w * sqrt(v.x*v.x + v.y*v.y + v.z*v.z));
	vn.x = v.x * s;
	vn.y = v.y * s;
	vn.z = v.z * s;
	vn.w = 1.0f;
	return vn;
}

const void vPrint(const std::string s, const vec4& v)
{
	std::cout << s << " = (" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")" << std::endl;
}

const bool vEqual(const vec4& v0, const vec4& v1)
{
	return (fabs(v0.x - v1.x) < vThreshold && fabs(v0.y - v1.y) < vThreshold &&
		fabs(v0.z - v1.z) < vThreshold && fabs(v0.w - v1.w) < vThreshold);
}

/////////////////////////////////////////////////////////////////////// mat4

typedef float mat4[16]; // column major
const float mThreshold = (float)1.0e-5;

const vec4 mMultiply(mat4 m, vec4& v)
{
	vec4 r;
	r.x = m[0] * v.x + m[4] * v.y + m[8] * v.z + m[12] * v.w;
	r.y = m[1] * v.x + m[5] * v.y + m[9] * v.z + m[13] * v.w;
	r.z = m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14] * v.w;
	r.w = m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15] * v.w;
	return r;
}

const void mClean(mat4 m)
{
	for (int i = 0; i < 16; i++) {
		if (fabs(m[i]) < mThreshold) m[i] = 0.0f;
	}
}

const void mPrint(const std::string s, const mat4 m)
{
	std::cout << s << " =" << std::endl;
	std::cout << std::fixed << std::setprecision(3) << "| ";
	std::cout.width(6); std::cout << m[0] << " ";
	std::cout.width(6); std::cout << m[4] << " ";
	std::cout.width(6); std::cout << m[8] << " ";
	std::cout.width(6); std::cout << m[12] << " |" << std::endl << "| ";
	std::cout.width(6); std::cout << m[1] << " ";
	std::cout.width(6); std::cout << m[5] << " ";
	std::cout.width(6); std::cout << m[9] << " ";
	std::cout.width(6); std::cout << m[13] << " |" << std::endl << "| ";
	std::cout.width(6); std::cout << m[2] << " ";
	std::cout.width(6); std::cout << m[6] << " ";
	std::cout.width(6); std::cout << m[10] << " ";
	std::cout.width(6); std::cout << m[14] << " |" << std::endl << "| ";
	std::cout.width(6); std::cout << m[3] << " ";
	std::cout.width(6); std::cout << m[7] << " ";
	std::cout.width(6); std::cout << m[11] << " ";
	std::cout.width(6); std::cout << m[15] << " |" << std::endl;
}

/////////////////////////////////////////////////////////////////////// QUATERNION

typedef struct {
	float t, x, y, z;
} qtrn;

const float qThreshold = (float)1.0e-5;

const qtrn qFromAngleAxis(float theta, vec4 axis);
const void qToAngleAxis(const qtrn& q, float& theta, vec4& axis);
const void qClean(qtrn& q);
const float qQuadrance(const qtrn& q);
const float qNorm(const qtrn& q);
const qtrn qNormalize(const qtrn& q);
const qtrn qConjugate(const qtrn& q);
const qtrn qInverse(const qtrn& q);
const qtrn qMultiply(const qtrn& q, const float s);
const qtrn qMultiply(const qtrn& q0, const qtrn& q1);
const void qGLMatrix(const qtrn& q, mat4 matrix);
const qtrn qLerp(const qtrn& q0, const qtrn& q1, float k);
const qtrn qSlerp(const qtrn& q0, const qtrn& q1, float k);
const bool qEqual(const qtrn& q0, const qtrn& q1);
const void qPrint(const std::string& s, const qtrn& q);
const void qPrintAngleAxis(const std::string& s, const qtrn& q);

const qtrn qFromAngleAxis(float theta, vec4 axis)
{
	vec4 axisn = vNormalize(axis);

	qtrn q;
	float a = theta * (float)DEGREES_TO_RADIANS;
	q.t = cos(a / 2.0f);
	float s = sin(a / 2.0f);
	q.x = axisn.x * s;
	q.y = axisn.y * s;
	q.z = axisn.z * s;

	qClean(q);
	return qNormalize(q);
}

const void qToAngleAxis(const qtrn& q, float& theta, vec4& axis)
{
	qtrn qn = qNormalize(q);
	theta = 2.0f * acos(qn.t) * (float)RADIANS_TO_DEGREES;
	float s = sqrt(1.0f - qn.t*qn.t);
	if (s < qThreshold) {
		axis.x = 1.0f;
		axis.y = 0.0f;
		axis.z = 0.0f;
		axis.w = 1.0f;
	}
	else {
		axis.x = qn.x / s;
		axis.y = qn.y / s;
		axis.z = qn.z / s;
		axis.w = 1.0f;
	}
}

const void qClean(qtrn& q)
{
	if (fabs(q.t) < qThreshold) q.t = 0.0f;
	if (fabs(q.x) < qThreshold) q.x = 0.0f;
	if (fabs(q.y) < qThreshold) q.y = 0.0f;
	if (fabs(q.z) < qThreshold) q.z = 0.0f;
}

const float qQuadrance(const qtrn& q)
{
	return q.t*q.t + q.x*q.x + q.y*q.y + q.z*q.z;
}

const float qNorm(const qtrn& q)
{
	return sqrt(qQuadrance(q));
}

const qtrn qNormalize(const qtrn& q)
{
	float s = 1 / qNorm(q);
	return qMultiply(q, s);
}

const qtrn qConjugate(const qtrn& q)
{
	qtrn qconj = { q.t, -q.x, -q.y, -q.z };
	return qconj;
}

const qtrn qInverse(const qtrn& q)
{
	return qMultiply(qConjugate(q), 1.0f / qQuadrance(q));
}

const qtrn qAdd(const qtrn& q0, const qtrn& q1)
{
	qtrn q;
	q.t = q0.t + q1.t;
	q.x = q0.x + q1.x;
	q.y = q0.y + q1.y;
	q.z = q0.z + q1.z;
	return q;
}

const qtrn qMultiply(const qtrn& q, const float s)
{
	qtrn sq;
	sq.t = s * q.t;
	sq.x = s * q.x;
	sq.y = s * q.y;
	sq.z = s * q.z;
	return sq;
}

const qtrn qMultiply(const qtrn& q0, const qtrn& q1)
{
	qtrn q;
	q.t = q0.t * q1.t - q0.x * q1.x - q0.y * q1.y - q0.z * q1.z;
	q.x = q0.t * q1.x + q0.x * q1.t + q0.y * q1.z - q0.z * q1.y;
	q.y = q0.t * q1.y + q0.y * q1.t + q0.z * q1.x - q0.x * q1.z;
	q.z = q0.t * q1.z + q0.z * q1.t + q0.x * q1.y - q0.y * q1.x;
	return q;
}

const void qGLMatrix(const qtrn& q, mat4 matrix)
{
	qtrn qn = qNormalize(q);

	float xx = qn.x * qn.x;
	float xy = qn.x * qn.y;
	float xz = qn.x * qn.z;
	float xt = qn.x * qn.t;
	float yy = qn.y * qn.y;
	float yz = qn.y * qn.z;
	float yt = qn.y * qn.t;
	float zz = qn.z * qn.z;
	float zt = qn.z * qn.t;

	matrix[0] = 1.0f - 2.0f * (yy + zz);
	matrix[1] = 2.0f * (xy + zt);
	matrix[2] = 2.0f * (xz - yt);
	matrix[3] = 0.0f;

	matrix[4] = 2.0f * (xy - zt);
	matrix[5] = 1.0f - 2.0f * (xx + zz);
	matrix[6] = 2.0f * (yz + xt);
	matrix[7] = 0.0f;

	matrix[8] = 2.0f * (xz + yt);
	matrix[9] = 2.0f * (yz - xt);
	matrix[10] = 1.0f - 2.0f * (xx + yy);
	matrix[11] = 0.0f;

	matrix[12] = 0.0f;
	matrix[13] = 0.0f;
	matrix[14] = 0.0f;
	matrix[15] = 1.0f;

	mClean(matrix);
}

const qtrn qLerp(const qtrn& q0, const qtrn& q1, float k)
{
	float cos_angle = q0.x*q1.x + q0.y*q1.y + q0.z*q1.z + q0.t*q1.t;
	float k0 = 1.0f - k;
	float k1 = (cos_angle > 0) ? k : -k;
	qtrn qi = qAdd(qMultiply(q0, k0), qMultiply(q1, k1));
	return qNormalize(qi);
}

const qtrn qSlerp(const qtrn& q0, const qtrn& q1, float k)
{
	float angle = acos(q0.x*q1.x + q0.y*q1.y + q0.z*q1.z + q0.t*q1.t);
	float k0 = sin((1 - k)*angle) / sin(angle);
	float k1 = sin(k*angle) / sin(angle);
	qtrn qi = qAdd(qMultiply(q0, k0), qMultiply(q1, k1));
	return qNormalize(qi);
}

const bool qEqual(const qtrn& q0, const qtrn& q1)
{
	return (fabs(q0.t - q1.t) < qThreshold && fabs(q0.x - q1.x) < qThreshold &&
		fabs(q0.y - q1.y) < qThreshold && fabs(q0.z - q1.z) < qThreshold);
}

const void qPrint(const std::string& s, const qtrn& q)
{
	std::cout << s << " = (" << q.t << ", " << q.x << ", " << q.y << ", " << q.z << ")" << std::endl;
}

const void qPrintAngleAxis(const std::string& s, const qtrn& q)
{
	std::cout << s << " = [" << std::endl;

	float thetaf;
	vec4 axis_f;
	qToAngleAxis(q, thetaf, axis_f);
	std::cout << "  angle = " << thetaf << std::endl;
	vPrint("  axis", axis_f);
	std::cout << "]" << std::endl;
}

///////////////////////////////////////////////////////////////////////

#define HEADER(X) std::cout<<std::endl<<(X)<<std::endl<<std::endl;

void qtest1()
{
	HEADER("TEST 1 : Rotation of 90 degrees about the y-axis")

		vec4 axis = { 0.0f, 1.0f, 0.0f, 1.0f };
	qtrn q = qFromAngleAxis(90.0f, axis);
	qPrint("   q", q);

	qtrn vi = { 0.0f, 7.0f, 0.0f, 0.0f };
	qPrint("  vi", vi);

	qtrn qe = { 0.0f, 0.0f, 0.0f, -7.0f };
	qPrint("  qe", qe);

	qtrn qinv = qInverse(q);
	qClean(qinv);
	qPrint("qinv", qinv);

	qtrn qf = qMultiply(qMultiply(q, vi), qinv);
	qPrint("  qf", qf);

	assert(qEqual(qf, qe));
}

void qtest2()
{
	HEADER("TEST 2 : Rotation of 90 degrees about the y-axis with matrix")

		vec4 axis = { 0.0f, 1.0f, 0.0f, 1.0f };
	qtrn q = qFromAngleAxis(90.0f, axis);
	qPrint(" q", q);

	vec4 vi = { 7.0f, 0.0f, 0.0f, 1.0f };
	vPrint("vi", vi);

	vec4 ve = { 0.0f, 0.0f, -7.0f, 1.0f };
	vPrint("ve", ve);

	mat4 m;
	qGLMatrix(q, m);
	mPrint(" M", m);

	vec4 vf = mMultiply(m, vi);
	vPrint("vf", vf);

	assert(vEqual(vf, ve));
}

void qtest3()
{
	HEADER("TEST 3 : Yaw 900 = Roll 180 + Pitch 180")

		vec4 axis_x = { 1.0f, 0.0f, 0.0f, 1.0f };
	vec4 axis_y = { 0.0f, 1.0f, 0.0f, 1.0f };
	vec4 axis_z = { 0.0f, 0.0f, 1.0f, 1.0f };

	qtrn qyaw180 = qFromAngleAxis(900.0f, axis_y);
	qPrint("  qyaw180", qyaw180);

	qtrn qroll180 = qFromAngleAxis(180.0f, axis_x);
	qPrint(" qroll180", qroll180);

	qtrn qpitch180 = qFromAngleAxis(180.0f, axis_z);
	qPrint("qpitch180", qpitch180);

	qtrn qrp = qMultiply(qpitch180, qroll180);
	qPrint("      qrp", qrp);

	qtrn qpr = qMultiply(qroll180, qpitch180);
	qPrint("      qpr", qpr);

	qtrn qi = { 0.0f, 1.0f, 0.0f, 0.0f }; // x-axis
	qPrint("       qi", qi);

	qtrn qe = { 0.0f, -1.0f, 0.0f, 0.0f };
	qPrint("       qe", qe);

	qtrn qrpinv = qInverse(qrp);
	qtrn qfrp = qMultiply(qMultiply(qrp, qi), qrpinv);
	qPrint("     qfrp", qfrp);

	qtrn qprinv = qInverse(qpr);
	qtrn qfpr = qMultiply(qMultiply(qpr, qi), qprinv);
	qPrint("     qfpr", qfpr);

	assert(qEqual(qe, qfrp));
	assert(qEqual(qe, qfpr));
}

void qtest4()
{
	HEADER("TEST 4: Q <-> (angle, axis)")

		float thetai = 45.0f;
	vec4 axis_i = { 0.0f, 1.0f, 0.0f, 1.0f };
	qtrn q = qFromAngleAxis(thetai, axis_i);
	std::cout << thetai << " ";
	vPrint("axis_i", axis_i);

	float thetaf;
	vec4 axis_f;
	qToAngleAxis(q, thetaf, axis_f);
	std::cout << thetaf << " ";
	vPrint("axis_f", axis_f);

	assert(fabs(thetai - thetaf) < qThreshold && vEqual(axis_i, axis_f));
}

void qtest5()
{
	HEADER("TEST 5: LERP")

		vec4 axis = { 0.0f, 1.0f, 0.0f, 1.0f };
	qtrn q0 = qFromAngleAxis(0.0f, axis);
	qPrint("       q0", q0);

	qtrn q1 = qFromAngleAxis(90.0f, axis);
	qPrint("       q1", q1);

	qtrn qe = qFromAngleAxis(30.0f, axis);
	qPrint("       qe", qe);

	qtrn qLerp0 = qLerp(q0, q1, 0.0f);
	qPrint("  lerp(0)", qLerp0);
	assert(qEqual(qLerp0, q0));

	qtrn qLerp1 = qLerp(q0, q1, 1.0f);
	qPrint("  lerp(1)", qLerp1);
	assert(qEqual(qLerp1, q1));

	qtrn qlerp = qLerp(q0, q1, 1 / 3.0f);
	qPrint("lerp(1/3)", qlerp);
	qPrintAngleAxis("lerp(1/3)", qlerp);

	assert(qEqual(qlerp, qe) == 0);
}

void qtest6()
{
	HEADER("TEST 6: SLERP")

		vec4 axis = { 0.0f, 1.0f, 0.0f, 1.0f };
	qtrn q0 = qFromAngleAxis(0.0f, axis);
	qPrint("        q0", q0);

	qtrn q1 = qFromAngleAxis(90.0f, axis);
	qPrint("        q1", q1);

	qtrn qe = qFromAngleAxis(30.0f, axis);
	qPrint("        qe", qe);

	qtrn qSlerp0 = qSlerp(q0, q1, 0.0f);
	qPrint("  slerp(0)", qSlerp0);
	assert(qEqual(qSlerp0, q0));

	qtrn qSlerp1 = qSlerp(q0, q1, 1.0f);
	qPrint("  slerp(1)", qSlerp1);
	assert(qEqual(qSlerp1, q1));

	qtrn qslerp = qSlerp(q0, q1, 1 / 3.0f);
	qPrint("slerp(1/3)", qslerp);
	qPrintAngleAxis("slerp(1/3)", qslerp);

	assert(qEqual(qslerp, qe));
}

///////////////////////////////////////////////////////////////////////

//int main(int argc, char* argv[])
//{
//	qtest1();
//	qtest2();
//	qtest3();
//	qtest4();
//	qtest5();
//	qtest6();
//
//	exit(EXIT_SUCCESS);
//}

///////////////////////////////////////////////////////////////////////