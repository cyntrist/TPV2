#include "DeAcceleration.h"

#include "../ecs/Manager.h"
#include "../utils/Vector2D.h"
constexpr double FRICTION_FACTOR = 0.995f;

DeAcceleration::DeAcceleration() : transform_(nullptr)
{
}

void DeAcceleration::initComponent()
{
	transform_ = mngr_->getComponent<Transform>(ent_);
	assert(transform_ != nullptr);
}

void DeAcceleration::update()
{
	auto &vel = transform_->getVel();
	vel = vel * FRICTION_FACTOR;

	if (vel.magnitude() < 0.05f)
		vel = Vector2D(0,0);
}
