#pragma once
#include "BlackHoleUtils.h"
#include "GameState.h"
#include "MissileUtils.h"
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
	BlackHoleUtils* bh_utils_;
	MissileUtils* m_utils_;
	int last_asteroid_; // timer
	int last_missile_; // timer

public:
	RunningState(FighterUtils*, AsteroidsUtils*, BlackHoleUtils*, MissileUtils*);
	~RunningState() override;
	void enter() override;
	void update() override;
	void leave() override;
};
