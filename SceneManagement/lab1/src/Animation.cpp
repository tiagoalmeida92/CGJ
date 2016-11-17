#include "Animation.hpp"

Animation::Animation(SceneNode* node, Vec3& start, Vec3& end) {
	node_ = node;
	start_ = start;
	end_ = end;
	
}

Animation::Animation(SceneNode* node, Vec3& start, Vec3& end, float rollAngleStart, float rollAngleEnd) {
	node_ = node;
	start_ = start;
	end_ = end;
	roll_angleZ_start = rollAngleStart;
	roll_angleZ_end = rollAngleEnd;
}

Animation::Animation(SceneNode* node, Vec3& start, Vec3& end, Vec3& axis, float rollAngleStart, float rollAngleEnd) {
	node_ = node;
	start_ = start;
	end_ = end;
	axis_ = axis;
	roll_angleX_start = rollAngleStart;
	roll_angleX_end = rollAngleEnd;
}

Animation::Animation(SceneNode* node, Vec3& start, Vec3& end, float rollZAngleStart, float rollZAngleEnd, 
	float rollYAngleStart, float rollYAngleEnd) {
	node_ = node;
	start_ = start;
	end_ = end;
	roll_angleZ_start = rollZAngleStart;
	roll_angleZ_end = rollZAngleEnd;
	roll_angleY_start = rollYAngleStart;
	roll_angleY_end = rollYAngleEnd;
}

void Animation::animate(float i, bool reverse) {
	Vec3 position;
	if (!reverse) {
		position = lerp(start_, end_, i);
	}
	else {
		position = lerp(end_, start_, i);
	}
	cout << position << endl;
	float rotationZ;
	if (!reverse) {
		rotationZ = roll_angleZ_start + (roll_angleZ_end - roll_angleZ_start) * i;
	}
	else {
		rotationZ = roll_angleZ_end + (roll_angleZ_start - roll_angleZ_end) * i;
	}
	float rotationY;
	if (!reverse) {
		rotationY = roll_angleY_start + (roll_angleY_end - roll_angleY_start) * i;
	}
	else {
		rotationY = roll_angleY_end + (roll_angleY_start - roll_angleY_end) * i;
	}

	float rotationX;
	if (!reverse) {
		rotationX = roll_angleX_start + (roll_angleX_end - roll_angleX_start) * i;
	}
	else {
		rotationX = roll_angleX_end + (roll_angleX_start - roll_angleX_end) * i;
	}
	cout << rotationZ << endl;
	node_->setMatrix(
		translate(position)
		* rotate4(AXIS_X, rotationX)
		* rotate4(AXIS_Y, rotationY)
		* rotate4(AXIS_Z, rotationZ));
}