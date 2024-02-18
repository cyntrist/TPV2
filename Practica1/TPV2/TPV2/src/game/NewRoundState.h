#pragma once
#include "GameState.h"
#include "../sdlutils/Texture.h"
class FighterUtils;
class AsteroidsUtils;

class NewRoundState : public GameState
{
	FighterUtils* f_utils_;
	AsteroidsUtils* a_utils_;
	Texture* message_;
	SDL_Rect rect_;

public:
	NewRoundState(FighterUtils*, AsteroidsUtils*);
	~NewRoundState() override;
	void enter() override;
	void update() override;
	void leave() override;
};
