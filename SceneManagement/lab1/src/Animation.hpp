#pragma once

#include "vec.hpp"
#include "Scene.hpp"

class Animation {
public:
	Animation(SceneNode* node, Vec3& start, Vec3& end);

	void animate(float i, bool reverse);


	SceneNode* node_;
	Vec3 start_;
	Vec3 end_;
};