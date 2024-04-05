// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <vector>

#include "../ecs/ecs.h"

class GhostSystem;
class ImmunitySystem;
class CollisionsSystem;
class RenderSystem;
class FruitsSystem;
class GameCtrlSystem;
class PacManSystem;
class GameState;

class Game {
	enum State
	{
		NEWGAME,
		NEWROUND,
		RUNNING,
		PAUSED,
		GAMEOVER
	};

public:
	Game();
	virtual ~Game();
	void init();
	void start();

	void setState(State);

private:
	ecs::Manager *mngr_;

	PacManSystem *pacmanSys_;
	GameCtrlSystem *gameCtrlSys_;
	FruitsSystem *fruitsSys_;
	RenderSystem *renderSys_;
	CollisionsSystem *collisionSys_;
	ImmunitySystem *immunitySys_;
	GhostSystem *ghostSys_;

	GameState* newGameState;
	GameState* newRoundState;
	GameState* pauseState;
	GameState* runningState;
	GameState* gameOverState;
	GameState* currentState;
};

