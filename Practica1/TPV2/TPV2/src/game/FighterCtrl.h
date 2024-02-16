#pragma once
#include <SDL_scancode.h>
#include "../sdlutils/InputHandler.h"
#include <SDL.h>
#include "../ecs/Component.h"

class FighterCtrl : public ecs::Component
{
public:
	FighterCtrl();
	virtual ~FighterCtrl() override = default;
	void handleInput();

private:
	SDL_Scancode up_;
	SDL_Scancode left_;
	SDL_Scancode right_;
};
