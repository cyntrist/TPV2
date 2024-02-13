#include "FighterCtrl.h";

constexpr double THRUST_FACTOR = 0.2f,
				 SPEED_LIMIT = 3.0f;	

FighterCtrl::FighterCtrl() :
up_ (SDL_SCANCODE_UP),
left_(SDL_SCANCODE_LEFT),
right_(SDL_SCANCODE_RIGHT)
{
}

void FighterCtrl::handleInput(Container* o)
{
	auto &ihdlr = ih();

	if (ihdlr.keyDownEvent()) {
		if (ihdlr.isKeyDown(left_)) {
			o->setRotation(o->getRotation() - 5.0f);
		} else if (ihdlr.isKeyDown(right_)) {
			o->setRotation(o->getRotation() + 5.0f);
		} else if (ihdlr.isKeyDown(up_))
		{
			auto &vel = o->getVel();
			vel = vel + Vector2D(0,-1).rotate(o->getRotation())*THRUST_FACTOR;
			if (vel.getX() > SPEED_LIMIT || vel.getY() > SPEED_LIMIT)
				vel = vel.normalize()*SPEED_LIMIT;
		}
	}
}
