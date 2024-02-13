#include "FighterCtrl.h";

FighterCtrl::FighterCtrl() :
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
		}
	}
}
