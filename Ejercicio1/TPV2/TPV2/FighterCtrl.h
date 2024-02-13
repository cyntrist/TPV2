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
	void handleInput(Container *o) override;

private:
	SDL_Scancode up_;
	SDL_Scancode left_;
	SDL_Scancode right_;
};
