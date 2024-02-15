#include "FighterCtrl.h";
#include "../sdlutils/SDLUtils.h"

constexpr double THRUST_FACTOR = 0.2f;
constexpr double ROTATION_MOVE = 5.0f;

FighterCtrl::FighterCtrl() :
up_ (SDL_SCANCODE_UP),
left_(SDL_SCANCODE_LEFT),
right_(SDL_SCANCODE_RIGHT)
{
}

void FighterCtrl::handleInput(Container* o)
{
	auto &ihdlr = ih();

	if (ihdlr.isKeyDown(left_)) {
		o->setRotation(o->getRotation() - ROTATION_MOVE);
	}
	else if (ihdlr.isKeyDown(right_)) {
		o->setRotation(o->getRotation() + ROTATION_MOVE);
	} 
	if (ihdlr.isKeyDown(up_))
	{
		sdlutils().soundEffects().at("thrust").play();
		auto &vel = o->getVel();
		vel = vel + Vector2D(0,-1).rotate(o->getRotation())*THRUST_FACTOR;
	}
}
