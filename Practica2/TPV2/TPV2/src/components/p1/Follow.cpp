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
	const auto chance = sdlutils().rand().nextInt(0, 1000);
	if (chance < 5) // 5 veces de cada 1000 
	{
		// cambia de destino
		auto pacman = mngr_->getHandler(ecs::hdlr::PACMAN);
		destination_ = mngr_->getComponent<Transform>(pacman)->pos_;
		auto diff = destination_ - pos;
		vel = diff.normalize() * 1.1f;
		trans_->vel_ = vel;
	}

	/// rebote en bordes
	if (pos.getX() - trans_->width_ <= 0
		|| pos.getX() + trans_->width_ >= sdlutils().width()
		&& round(abs(vel.getX())) != 0.00)
		vel.setX(vel.getX() * -1);


	if (pos.getY() - trans_->height_ <= 0
		|| pos.getY() + trans_->height_ >= sdlutils().height()
		&& round(abs(vel.getY())) != 0.00)
		vel.setY(vel.getY() * -1);

	/// movimiento
	trans_->pos_ = trans_->pos_ + trans_->vel_;
}

void Follow::invertSpeed()
{
	trans_->vel_.set(trans_->vel_.getX() * -1, trans_->vel_.getY() * -1);
}
