#pragma once
#include <SDL_rect.h>
#include "../sdlutils/Texture.h"
#include "GameState.h"

class GameOverState : public GameState
{
	Texture* message_;
	SDL_Rect rect_;

public:
	GameOverState();
	~GameOverState() override;
	void enter() override;
	void update() override;
	void leave() override;
};
