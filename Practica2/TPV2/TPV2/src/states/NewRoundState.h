#pragma once
#include "GameState.h"
#include "../sdlutils/Texture.h"

class NewRoundState : public GameState
{
	Texture* message_;
	SDL_Rect rect_;

public:
	NewRoundState();
	~NewRoundState() override;
	void enter() override;
	void update() override;
	void leave() override;
};
