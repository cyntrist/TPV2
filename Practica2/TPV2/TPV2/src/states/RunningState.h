#pragma once
#include "GameState.h"
#include "../ecs/Manager.h"


class RenderSystem;
class CollisionsSystem;
class ImmunitySystem;
class FruitsSystem;
class GhostSystem;
class PacManSystem;

class RunningState : public GameState
{
	void checkCollisions();
	void onDeath();
	PacManSystem *pacmanSys_;
	//ecs::System *gameCtrlSys_;
	GhostSystem *ghostSys_;
	FruitsSystem *fruitsSys_;
	ImmunitySystem *immunitySys_;
	RenderSystem *renderSys_;
	CollisionsSystem *collisionSys_;
	

public:
	RunningState(PacManSystem* pc, GhostSystem*gh, FruitsSystem*fo,
	ImmunitySystem*im, RenderSystem*r, CollisionsSystem*col);
	~RunningState() override;
	void enter() override;
	void update() override;
	void leave() override;
};
