#include "TowardDestination.h"
#include "../sdlutils/SDLUtils.h"
#include "../ecs/Manager.h"

constexpr float STOP_DISTANCE = 10.0f;

TowardDestination::TowardDestination() :
	trans_(nullptr),
	destination_(),
	random_(sdlutils().rand())
{
}

void TowardDestination::initComponent()
{
	trans_ = mngr_->getComponent<Transform>(ent_);
	assert(trans_ != nullptr);
	steer();
}

void TowardDestination::update()
{
	Vector2D diff = destination_ - trans_->getPos();
	if (diff.magnitude() < STOP_DISTANCE)
		steer();
}

void TowardDestination::steer()
{
	auto& pos = trans_->getPos();
	auto& vel = trans_->getVel();
	destination_ = Vector2D(
		random_.nextInt(0, sdlutils().width()),
		random_.nextInt(0, sdlutils().height())
	);
	Vector2D diff = destination_ - pos;
	vel = vel.rotate(vel.angle(diff));
}
