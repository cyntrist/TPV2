// This file is part of the course TPV2@UCM - Samir Genaim

#include "Game.h"

#include "../ecs/Manager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../states/NewGameState.h"
#include "../states/NewRoundState.h"
#include "../states/PausedState.h"
#include "../states/RunningState.h"
#include "../states/GameOverState.h"
#include "../systems/CollisionsSystem.h"
#include "../systems/GameCtrlSystem.h"
#include "../systems/PacManSystem.h"
#include "../systems/RenderSystem.h"
#include "../systems/FruitsSystem.h"
#include "../utils/Vector2D.h"
#include "../utils/Collisions.h"
#include "../systems/GhostSystem.h"
#include "../systems/ImmunitySystem.h"

using ecs::Manager;

Game::Game() :
		mngr_(), //
		pacmanSys_(), //
		gameCtrlSys_(), //
		fruitsSys_(), //
		renderSys_(), //
		collisionSys_(),
		newGameState(),
		newRoundState(),
		pauseState(),
		runningState(),
		gameOverState()
{
}

Game::~Game() {
	delete newGameState;
	delete newRoundState;
	delete pauseState;
	delete runningState;
	delete gameOverState;
	delete mngr_;
}

void Game::init() {
	// initialise the SDLUtils singleton
	SDLUtils::init("Demo", 800, 600, "resources/config/resources.json");

	// Create the manager
	mngr_ = new Manager();

	// add the systems
	pacmanSys_ = mngr_->addSystem<PacManSystem>();
	fruitsSys_ = mngr_->addSystem<FruitsSystem>();
	//gameCtrlSys_ = mngr_->addSystem<GameCtrlSystem>();
	renderSys_ = mngr_->addSystem<RenderSystem>();
	collisionSys_ = mngr_->addSystem<CollisionsSystem>();
	immunitySys_ = mngr_->addSystem<ImmunitySystem>();
	ghostSys_ = mngr_->addSystem<GhostSystem>();

	// create the states
	newGameState = new NewGameState();
	newRoundState = new NewRoundState();
	pauseState = new PausedState();
	runningState = new RunningState(
		pacmanSys_, ghostSys_, fruitsSys_, 
		immunitySys_, renderSys_, collisionSys_
	);
	gameOverState = new GameOverState();

	newGameState->setContext(mngr_);
	newRoundState->setContext(mngr_);
	pauseState->setContext(mngr_);
	runningState->setContext(mngr_);
	gameOverState->setContext(mngr_);

	setState(RUNNING);
}

void Game::start() {

	// a boolean to exit the loop
	bool exit = false;

	auto &ihdlr = ih();

	while (!exit) {
		Uint32 startTime = sdlutils().currRealTime();

		// refresh the input handler
		ihdlr.refresh();

		if (ihdlr.isKeyDown(SDL_SCANCODE_ESCAPE)) {
			exit = true;
			continue;
		}

		sdlutils().clearRenderer();
		currentState->update();
		mngr_->flushMessagesWithSwap();
		mngr_->refresh();
		sdlutils().presentRenderer();

		Uint32 frameTime = sdlutils().currRealTime() - startTime;
		if (frameTime < 10)
			SDL_Delay(10 - frameTime);
	}
}

void Game::setState(State newState)
{
	if (currentState != nullptr) 
		currentState->leave();
	switch (newState)
	{
	case NEWGAME:	currentState = newGameState; break;
	case NEWROUND:	currentState = newRoundState; break;
	case RUNNING:	currentState = runningState; break;
	case PAUSED:	currentState = pauseState; break;
	case GAMEOVER:	currentState = gameOverState; break;
	default: break;
	}
	if (currentState != nullptr)
		currentState->enter();
}

