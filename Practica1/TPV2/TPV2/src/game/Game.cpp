// This file is part of the course TPV2@UCM - Samir Genaim

#include "Game.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../ecs/Entity.h"
#include "AsteroidsUtils.h"
#include "FighterUtils.h"
#include "GameOverState.h"
#include "NewGameState.h"
#include "NewRoundState.h"
#include "PausedState.h"
#include "RunningState.h"

Game::Game()
	: mngr_(nullptr)
{
}

Game::~Game()
{
	delete mngr_;
	delete f_utils_;
	delete a_utils_;
	delete bh_utils_;
	delete m_utils_;
	delete newgame_state_;
	delete newround_state_;
	delete running_state_;
	delete gameover_state_;
	delete paused_state_;
}

void Game::init()
{
	// initialize the SDL singleton
	SDLUtils::init("Asteroids", 800, 600,
	               "resources/config/asteroid.resources.json");

	// Manager
	mngr_ = new ecs::Manager();
	// Fighter
	f_utils_ = new FighterUtils();
	f_utils_->create_fighter();
	//Asteroids
	a_utils_ = new AsteroidsUtils();

	// Black Holes
	bh_utils_ = new BlackHoleUtils();
	// Missiles
	m_utils_ = new MissileUtils();

	// States
	newgame_state_ = new NewGameState(f_utils_);
	newround_state_ = new NewRoundState(f_utils_, a_utils_, bh_utils_, m_utils_);
	running_state_ = new RunningState(f_utils_, a_utils_, bh_utils_, m_utils_);
	gameover_state_ = new GameOverState();
	paused_state_ = new PausedState();

	setState(NEWGAME);
}

void Game::start()
{
	// a boolean to exit the loop
	bool exit = false;

	auto& ihdlr = ih();

	while (!exit)
	{
		Uint32 startTime = sdlutils().currRealTime();

		// refresh the input handler
		ihdlr.refresh();

		if (ihdlr.isKeyDown(SDL_SCANCODE_ESCAPE))
		{
			exit = true;
			continue;
		}

		current_state_->update();

		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 10)
			SDL_Delay(10 - frameTime);
	}
}
