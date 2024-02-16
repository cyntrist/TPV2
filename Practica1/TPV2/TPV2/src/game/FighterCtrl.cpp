#include "FighterCtrl.h";
#include "../sdlutils/SDLUtils.h"
#include "../ecs/Manager.h"

constexpr double THRUST_FACTOR = 0.2f;
constexpr double ROTATION_MOVE = 5.0f;

FighterCtrl::FighterCtrl() :
up_ (SDL_SCANCODE_UP),
left_(SDL_SCANCODE_LEFT),
right_(SDL_SCANCODE_RIGHT)
{
}

void FighterCtrl::initComponent()
{
	transform_ = mngr_->getComponent<Transform>(ent_);
	assert(transform_ != nullptr);
}

void FighterCtrl::update()
{
	auto &ihdlr = ih();

	if (ihdlr.isKeyDown(left_)) {
		transform_->setRotation(transform_->getRot() - ROTATION_MOVE);
	}
	else if (ihdlr.isKeyDown(right_)) {
		transform_->setRotation(transform_->getRot() + ROTATION_MOVE);
	} 
	if (ihdlr.isKeyDown(up_))
	{
		sdlutils().soundEffects().at("thrust").play();
		auto &vel = transform_->getVel();
		vel = vel + Vector2D(0,-1).rotate(transform_->getRot())*THRUST_FACTOR;
	}
}
