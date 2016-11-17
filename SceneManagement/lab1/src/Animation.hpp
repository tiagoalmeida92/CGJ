#pragma once

#include "vec.hpp"
#include "Scene.hpp"

class Animation {
public:
	Animation(SceneNode* node, Vec3& start, Vec3& end);
	Animation(SceneNode* node, Vec3& start, Vec3& end, float rollAngleStart, float rollAngleEnd);
	Animation(SceneNode* node, Vec3& start, Vec3& end, Vec3& axis, float rollAngleStart, float rollAngleEnd);
	Animation(SceneNode* node, Vec3& start, Vec3& end, float rollZAngleStart, float rollZAngleEnd, float rollYAngleStart, float rollYAngleEnd);

	void animate(float i, bool reverse);


	SceneNode* node_;
	Vec3 start_;
	Vec3 end_;
	Vec3 axis_ = AXIS_Z; //default rotation roll
	float roll_angleX_start = 0;
	float roll_angleX_end = 0;
	float roll_angleY_start = 0;
	float roll_angleY_end = 0;
	float roll_angleZ_start = 0;
	float roll_angleZ_end = 0;

};