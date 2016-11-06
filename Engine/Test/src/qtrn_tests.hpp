
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

#define qPrint(X, Y) print(Y)

void qtest3()
{
	HEADER("TEST 3 : Yaw 900 = Roll 180 + Pitch 180")

	Vec4 axis_x = { 1.0f, 0.0f, 0.0f, 1.0f };
	Vec4 axis_y = { 0.0f, 1.0f, 0.0f, 1.0f };
	Vec4 axis_z = { 0.0f, 0.0f, 1.0f, 1.0f };

	qtrn qyaw180 = fromAngleAxis(900.0f, axis_y);
	print(qyaw180);

	qtrn qroll180 = fromAngleAxis(180.0f, axis_x);
	qPrint(" qroll180", qroll180);

	qtrn qpitch180 = fromAngleAxis(180.0f, axis_z);
	qPrint("qpitch180", qpitch180);

	qtrn qrp = qpitch180 * qroll180;
	qPrint("      qrp", qrp);

	qtrn qpr = qroll180 * qpitch180;
	qPrint("      qpr", qpr);

	qtrn qi = { 0.0f, 1.0f, 0.0f, 0.0f }; // x-axis
	qPrint("       qi", qi);

	qtrn qe = { 0.0f, -1.0f, 0.0f, 0.0f };
	qPrint("       qe", qe);

	qtrn qrpinv = qrp.inverse();
	qtrn qfrp = (qrp * qi) * qrpinv;
	qPrint("     qfrp", qfrp);

	qtrn qprinv = qpr.inverse();
	qtrn qfpr = (qpr * qi) * qprinv;
	qPrint("     qfpr", qfpr);

	assert(qe == qfrp);
	assert(qe == qfpr);
}
//
//void qtest4()
//{
//	HEADER("TEST 4: Q <-> (angle, axis)")
//
//		float thetai = 45.0f;
//	vec4 axis_i = { 0.0f, 1.0f, 0.0f, 1.0f };
//	qtrn q = qFromAngleAxis(thetai, axis_i);
//	std::cout << thetai << " ";
//	vPrint("axis_i", axis_i);
//
//	float thetaf;
//	vec4 axis_f;
//	qToAngleAxis(q, thetaf, axis_f);
//	std::cout << thetaf << " ";
//	vPrint("axis_f", axis_f);
//
//	assert(fabs(thetai - thetaf) < qThreshold && vEqual(axis_i, axis_f));
//}
//
//void qtest5()
//{
//	HEADER("TEST 5: LERP")
//
//		vec4 axis = { 0.0f, 1.0f, 0.0f, 1.0f };
//	qtrn q0 = qFromAngleAxis(0.0f, axis);
//	qPrint("       q0", q0);
//
//	qtrn q1 = qFromAngleAxis(90.0f, axis);
//	qPrint("       q1", q1);
//
//	qtrn qe = qFromAngleAxis(30.0f, axis);
//	qPrint("       qe", qe);
//
//	qtrn qLerp0 = qLerp(q0, q1, 0.0f);
//	qPrint("  lerp(0)", qLerp0);
//	assert(qEqual(qLerp0, q0));
//
//	qtrn qLerp1 = qLerp(q0, q1, 1.0f);
//	qPrint("  lerp(1)", qLerp1);
//	assert(qEqual(qLerp1, q1));
//
//	qtrn qlerp = qLerp(q0, q1, 1 / 3.0f);
//	qPrint("lerp(1/3)", qlerp);
//	qPrintAngleAxis("lerp(1/3)", qlerp);
//
//	assert(qEqual(qlerp, qe) == 0);
//}
//
//void qtest6()
//{
//	HEADER("TEST 6: SLERP")
//
//		vec4 axis = { 0.0f, 1.0f, 0.0f, 1.0f };
//	qtrn q0 = qFromAngleAxis(0.0f, axis);
//	qPrint("        q0", q0);
//
//	qtrn q1 = qFromAngleAxis(90.0f, axis);
//	qPrint("        q1", q1);
//
//	qtrn qe = qFromAngleAxis(30.0f, axis);
//	qPrint("        qe", qe);
//
//	qtrn qSlerp0 = qSlerp(q0, q1, 0.0f);
//	qPrint("  slerp(0)", qSlerp0);
//	assert(qEqual(qSlerp0, q0));
//
//	qtrn qSlerp1 = qSlerp(q0, q1, 1.0f);
//	qPrint("  slerp(1)", qSlerp1);
//	assert(qEqual(qSlerp1, q1));
//
//	qtrn qslerp = qSlerp(q0, q1, 1 / 3.0f);
//	qPrint("slerp(1/3)", qslerp);
//	qPrintAngleAxis("slerp(1/3)", qslerp);
//
//	assert(qEqual(qslerp, qe));
//}
