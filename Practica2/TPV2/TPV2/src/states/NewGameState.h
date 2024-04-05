#pragma once
#include <SDL_rect.h>
#include "../sdlutils/Texture.h"
#include "GameState.h"
class FighterUtils;

class NewGameState : public GameState
{
	FighterUtils* f_utils_;
	Texture* message_;
	SDL_Rect rect_;

public:
	NewGameState(FighterUtils*);
	~NewGameState() override;
	void enter() override;
	void update() override;
	void leave() override;
};
