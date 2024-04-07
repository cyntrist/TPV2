#include "Follow.h"
#include "../../ecs/Manager.h"
#include "../../sdlutils/SDLUtils.h"
class Vector2D;

Follow::Follow(Vector2D& dest)
	: trans_(nullptr), destination_(dest)
{
}


void Follow::initComponent()
{
	trans_ = mngr_->getComponent<Transform>(ent_);
	assert(trans_ != nullptr);
}

void Follow::update()
{
	auto pos = trans_->pos_;
	auto vel = trans_->vel_;
	const auto currTime = sdlutils().currRealTime();

	/// cambia de destino?
	if (timer + followDuration < currTime) 
	{ // cambia de destino
		auto pacman = mngr_->getHandler(ecs::hdlr::PACMAN);
		destination_ = mngr_->getComponent<Transform>(pacman)->pos_;
		auto diff = destination_ - pos;
		vel = diff.normalize() * 1.1f;
		trans_->vel_ = vel;
		followDuration = sdlutils().rand().nextInt(MIN_DURATION, MAX_DURATION);
		timer = currTime;
	}

	/// movimiento
	trans_->pos_ = trans_->pos_ + trans_->vel_;


	/// rebote
	if ((pos.getX() <= 0 && vel.getX() < -0.01) ||
		(pos.getX() >= sdlutils().width() - trans_->width_ 
			&& vel.getX() > 0.01))
	{
		vel.set(-vel.getX(), vel.getY());
	}
	else if ((vel.getY() <= 0 && vel.getY() < -0.01) ||
		(pos.getY() >= sdlutils().height() - trans_->height_
			&& vel.getY() > 0.01))
	{
		vel.set(vel.getX(), -vel.getY());
	}
}
