#pragma once
#include "GameState.h"
#include "../ecs/Manager.h"
class AsteroidsUtils;
class FighterUtils;
class RunningState : public GameState
{
	void checkCollisions();
	void onDeath();
	ecs::Manager* mngr_;
	FighterUtils* f_utils_;
	AsteroidsUtils* a_utils_;
	int last_asteroid_;
public:
	RunningState(FighterUtils*, AsteroidsUtils*);
	~RunningState() override;
	void enter() override;
	void update() override;
	void leave() override;
};
