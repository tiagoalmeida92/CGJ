#include "Animation.hpp"

Animation::Animation(SceneNode* node, Vec3& start, Vec3& end) {
	node_ = node;
	start_ = start;
	end_ = end_;
	
}

void Animation::animate(float i, bool reverse) {
	Vec3 position;
	if (!reverse) {
		position = lerp(start_, end_, i);
	}
	else {
		position = lerp(end_, start_, i);
	}
	node_->setMatrix(
		translate(position));
}