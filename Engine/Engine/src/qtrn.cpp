#include "qtrn.hpp"

#define DEGREES_TO_RADIANS 0.01745329251994329547
#define RADIANS_TO_DEGREES 57.29577951308232185913

qtrn::qtrn()
{
	t = 1;
}

qtrn::qtrn(float t, float x, float y, float z) {
	this->t = t;
	this->x = x;
	this->y = y;
	this->z = z;
}


const qtrn fromAngleAxis(float theta, Vec4& axis)
{
	Vec4 axisn = axis.normalize();

	qtrn q;
	float a = theta * (float)DEGREES_TO_RADIANS;
	q.t = cos(a / 2.0f);
	float s = sin(a / 2.0f);
	q.x = axisn.x * s;
	q.y = axisn.y * s;
	q.z = axisn.z * s;

	q.clean();
	return q.normalize();
}

const void qtrn::toAngleAxis(float& theta, Vec4& axis)
{
	qtrn qn = normalize();
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

const void qtrn::clean()
{
	if (fabs(t) < qThreshold) t = 0.0f;
	if (fabs(x) < qThreshold) x = 0.0f;
	if (fabs(y) < qThreshold) y = 0.0f;
	if (fabs(z) < qThreshold) z = 0.0f;
}

const float qtrn::quadrance()
{
	return t*t + x*x + y*y + z*z;
}

const float qtrn::norm()
{
	return sqrt(quadrance());
}

const qtrn qtrn::normalize()
{
	float s = 1 / norm();
	return *this * s;
}

const qtrn qtrn::conjugate()
{
	qtrn qconj = { t, -x, -y, -z };
	return qconj;
}

const qtrn qtrn::inverse()
{
	return conjugate() * (1.0f / quadrance());
}

const qtrn operator + (const qtrn& q0, const qtrn& q1) {
	qtrn q;
	q.t = q0.t + q1.t;
	q.x = q0.x + q1.x;
	q.y = q0.y + q1.y;
	q.z = q0.z + q1.z;
	return q;
}

const qtrn operator *(const qtrn& q, const float s)
{
	qtrn sq;
	sq.t = s * q.t;
	sq.x = s * q.x;
	sq.y = s * q.y;
	sq.z = s * q.z;
	return sq;
}

const qtrn operator * (const qtrn& q0, const qtrn& q1)
{
	qtrn q;
	q.t = q0.t * q1.t - q0.x * q1.x - q0.y * q1.y - q0.z * q1.z;
	q.x = q0.t * q1.x + q0.x * q1.t + q0.y * q1.z - q0.z * q1.y;
	q.y = q0.t * q1.y + q0.y * q1.t + q0.z * q1.x - q0.x * q1.z;
	q.z = q0.t * q1.z + q0.z * q1.t + q0.x * q1.y - q0.y * q1.x;
	return q;
}

const Mat4 qtrn::toMatrix()
{
	qtrn qn = normalize();

	float xx = qn.x * qn.x;
	float xy = qn.x * qn.y;
	float xz = qn.x * qn.z;
	float xt = qn.x * qn.t;
	float yy = qn.y * qn.y;
	float yz = qn.y * qn.z;
	float yt = qn.y * qn.t;
	float zz = qn.z * qn.z;
	float zt = qn.z * qn.t;
	
	Mat4 matrix;

	matrix.data[0] = 1.0f - 2.0f * (yy + zz);
	matrix.data[1] = 2.0f * (xy - zt);
	matrix.data[2] = 2.0f * (xz + yt);
	matrix.data[3] = 0.0f;

	matrix.data[4] = 2.0f * (xy + zt);
	matrix.data[5] = 1.0f - 2.0f * (xx + zz);
	matrix.data[6] = 2.0f * (yz - xt);
	matrix.data[7] = 0.0f;

	matrix.data[8] = 2.0f * (xz - yt);
	matrix.data[9] = 2.0f * (yz + xt);
	matrix.data[10] = 1.0f - 2.0f * (xx + yy);
	matrix.data[11] = 0.0f;

	matrix.data[12] = 0.0f;
	matrix.data[13] = 0.0f;
	matrix.data[14] = 0.0f;
	matrix.data[15] = 1.0f;

	matrix.clean();
	return matrix;
}

const qtrn lerp(const qtrn& q0, const qtrn& q1, float k)
{
	float cos_angle = q0.x*q1.x + q0.y*q1.y + q0.z*q1.z + q0.t*q1.t;
	float k0 = 1.0f - k;
	float k1 = (cos_angle > 0) ? k : -k;
	qtrn qi = (q0 * k0) + (q1 * k1);
	return qi.normalize();
}

const qtrn slerp(const qtrn& q0, const qtrn& q1, float k)
{
	float angle = acos(q0.x*q1.x + q0.y*q1.y + q0.z*q1.z + q0.t*q1.t);
	float k0 = sin((1 - k)*angle) / sin(angle);
	float k1 = sin(k*angle) / sin(angle);
	qtrn qi = (q0 * k0) + (q1 * k1);
	return qi.normalize();
}

const bool operator == (const qtrn& q0, const qtrn& q1)
{
	return (fabs(q0.t - q1.t) < qThreshold && fabs(q0.x - q1.x) < qThreshold &&
		fabs(q0.y - q1.y) < qThreshold && fabs(q0.z - q1.z) < qThreshold);
}

ostream& operator << (ostream& out, const qtrn &q)
{
	out << "(" << q.t << ", " << q.x << ", " << q.y << ", " << q.z << ")" << std::endl;
	return out;
}

//const void qPrint(const std::string& s, const qtrn& q)
//{
//	std::cout << s << " = (" << q.t << ", " << q.x << ", " << q.y << ", " << q.z << ")" << std::endl;
//}
//
//const void qPrintAngleAxis(const std::string& s, const qtrn& q)
//{
//	std::cout << s << " = [" << std::endl;
//
//	float thetaf;
//	vec4 axis_f;
//	qToAngleAxis(q, thetaf, axis_f);
//	std::cout << "  angle = " << thetaf << std::endl;
//	vPrint("  axis", axis_f);
//	std::cout << "]" << std::endl;
//}