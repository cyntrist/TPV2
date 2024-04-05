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
	int last_asteroid_; // timer
	int last_missile_; // timer

public:
	RunningState();
	~RunningState() override;
	void enter() override;
	void update() override;
	void leave() override;
};
