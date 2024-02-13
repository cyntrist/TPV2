#pragma once
#include <SDL_scancode.h>
#include "src/game/Container.h"
#include "src/game/InputComponent.h"
#include "src/sdlutils/InputHandler.h"
#include <SDL.h>

class FighterCtrl : public InputComponent
{
public:
	FighterCtrl();
	virtual ~FighterCtrl() override = default;

	inline void setKeys(SDL_Scancode up, SDL_Scancode left, SDL_Scancode right, SDL_Scancode stop) {
		up_ = up;
		left_ = left;
		right_ = right;
		stop_ = stop;
	}

	void handleInput(Container *o) override;

private:
	SDL_Scancode up_;
	SDL_Scancode left_;
	SDL_Scancode right_;
	SDL_Scancode stop_;
};
