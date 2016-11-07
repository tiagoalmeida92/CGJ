
#include "qtrn.hpp"

#include <iostream>
#include <iomanip>
#include <string>
#include <cassert>

#define HEADER(X) std::cout<<std::endl<<(X)<<std::endl<<std::endl;
#define print(s) cout << #s" " << s << '\n'

void qtest1()
{
	HEADER("TEST 1 : Rotation of 90 degrees about the y-axis")

	Vec4 axis = { 0.0f, 1.0f, 0.0f, 1.0f };
	qtrn q = fromAngleAxis(90.0f, axis);
	print(q);

	qtrn vi = { 0.0f, 7.0f, 0.0f, 0.0f };
	print(vi);

	qtrn qe = { 0.0f, 0.0f, 0.0f, -7.0f };
	print(qe);

	qtrn qinv = q.inverse();
	qinv.clean();
	print(qinv);

	qtrn qf = (q * vi) * qinv;
	print(qf);

	assert(qf == qe);
}

void qtest2()
{
	HEADER("TEST 2 : Rotation of 90 degrees about the y-axis with matrix")

	Vec4 axis = { 0.0f, 1.0f, 0.0f, 1.0f };
	qtrn q = fromAngleAxis(90.0f, axis);
	print(q);

	Vec4 vi = { 7.0f, 0.0f, 0.0f, 1.0f };
	print(vi);

	Vec4 ve = { 0.0f, 0.0f, -7.0f, 1.0f };
	print(ve);

	Mat4 m = q.GLMatrix();
	print(m);
	Mat4 mTransposed = m.transpose();
	Vec4 vf = mTransposed * vi;
	print(vf);

	assert(vf == ve);
}

void qtest3()
{
	HEADER("TEST 3 : Yaw 900 = Roll 180 + Pitch 180")

	Vec4 axis_x = { 1.0f, 0.0f, 0.0f, 1.0f };
	Vec4 axis_y = { 0.0f, 1.0f, 0.0f, 1.0f };
	Vec4 axis_z = { 0.0f, 0.0f, 1.0f, 1.0f };

	qtrn qyaw180 = fromAngleAxis(900.0f, axis_y);
	print(qyaw180);

	qtrn qroll180 = fromAngleAxis(180.0f, axis_x);
	print(qroll180);

	qtrn qpitch180 = fromAngleAxis(180.0f, axis_z);
	print(qpitch180);

	qtrn qrp = qpitch180 * qroll180;
	print(qrp);

	qtrn qpr = qroll180 * qpitch180;
	print(qpr);

	qtrn qi = { 0.0f, 1.0f, 0.0f, 0.0f }; // x-axis
	print(qi);

	qtrn qe = { 0.0f, -1.0f, 0.0f, 0.0f };
	print(qe);

	qtrn qrpinv = qrp.inverse();
	qtrn qfrp = (qrp * qi) * qrpinv;
	print(qfrp);

	qtrn qprinv = qpr.inverse();
	qtrn qfpr = (qpr * qi) * qprinv;
	print(qfpr);

	assert(qe == qfrp);
	assert(qe == qfpr);
}

void qtest4()
{
	HEADER("TEST 4: Q <-> (angle, axis)")

	float thetai = 45.0f;
	Vec4 axis_i = { 0.0f, 1.0f, 0.0f, 1.0f };
	qtrn q = fromAngleAxis(thetai, axis_i);
	std::cout << thetai << " ";
	print("axis_i", axis_i);

	float thetaf;
	Vec4 axis_f;
	q.toAngleAxis(thetaf, axis_f);
	std::cout << thetaf << " ";
	print("axis_f", axis_f);

	assert(fabs(thetai - thetaf) < qThreshold && axis_i == axis_f);
}

const void qPrintAngleAxis(const std::string& s, qtrn& q)
{
	std::cout << s << " = [" << std::endl;

	float thetaf;
	Vec4 axis_f;
	q.toAngleAxis(thetaf, axis_f);
	std::cout << "  angle = " << thetaf << std::endl;
	print(axis_f);
	std::cout << "]" << std::endl;
}

void qtest5()
{
	HEADER("TEST 5: LERP")

	Vec4 axis = { 0.0f, 1.0f, 0.0f, 1.0f };
	qtrn q0 = fromAngleAxis(0.0f, axis);
	print(q0);

	qtrn q1 = fromAngleAxis(90.0f, axis);
	print(q1);

	qtrn qe = fromAngleAxis(30.0f, axis);
	print(qe);

	qtrn qLerp0 = lerp(q0, q1, 0.0f);
	print(qLerp0);
	assert(qLerp0 == q0);

	qtrn qLerp1 = lerp(q0, q1, 1.0f);
	print(qLerp1);
	assert(qLerp1 == q1);

	qtrn qlerp = lerp(q0, q1, 1 / 3.0f);
	print(qlerp);
	qPrintAngleAxis("lerp(1/3)", qlerp);

	assert((qlerp == qe) != true);
}

void qtest6()
{
	HEADER("TEST 6: SLERP")

	Vec4 axis = { 0.0f, 1.0f, 0.0f, 1.0f };
	qtrn q0 = fromAngleAxis(0.0f, axis);
	print(q0);

	qtrn q1 = fromAngleAxis(90.0f, axis);
	print(q1);

	qtrn qe = fromAngleAxis(30.0f, axis);
	print(qe);

	qtrn qSlerp0 = slerp(q0, q1, 0.0f);
	print(qSlerp0);
	assert(qSlerp0 == q0);

	qtrn qSlerp1 = slerp(q0, q1, 1.0f);
	print(qSlerp1);
	assert(qSlerp1 == q1);

	qtrn qslerp = slerp(q0, q1, 1 / 3.0f);
	print(qslerp);
	qPrintAngleAxis("slerp(1/3)", qslerp);

	assert(qslerp == qe);
}
