#pragma once
#include <SDL_rect.h>
#include "../sdlutils/Texture.h"
#include "GameState.h"

class PausedState : public GameState
{
	Texture* message_;
	SDL_Rect rect_;

public:
	PausedState();
	~PausedState() override;
	void enter() override;
	void update() override;
	void leave() override;
};
