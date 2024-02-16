// This file is part of the course TPV2@UCM - Samir Genaim

#include "Game.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Collisions.h"
#include "../ecs/Entity.h"
#include "ImageRenderer.h"
#include "InfoMsgs.h"
#include "ScoreRenderer.h"
#include "Transform.h"
#include "DeAcceleration.h"
#include "FighterCtrl.h"
#include "FighterUtils.h"
#include "Gun.h"
#include "HealthComponent.h"
#include "ShowAtOppositeSide.h"

Game::Game()
	: mngr_(nullptr)
{}

Game::~Game() {
	delete mngr_;
}

void Game::init() {
	// initialize the SDL singleton
	SDLUtils::init("Asteroids", 800, 600,
			"resources/config/asteroid.resources.json");

	// Manager
	mngr_ = new ecs::Manager();
	// Fighter
	auto fighterFacade = new FighterUtils();
	fighterFacade->create_fighter();
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

		mngr_->update();
		mngr_->refresh();

		checkCollisions();

		sdlutils().clearRenderer();
		mngr_->render();
		sdlutils().presentRenderer();

		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 10)
			SDL_Delay(10 - frameTime);
	}
}

void Game::checkCollisions() {
//	if (gm_->getState() != GameManager::RUNNING)
//		return;
//
//	// check if ball hits paddles
//	if (Collisions::collides(leftPaddle_->getPos(), leftPaddle_->getWidth(),
//			leftPaddle_->getHeight(), ball_->getPos(), ball_->getWidth(),
//			ball_->getHeight())
//			|| Collisions::collides(rightPaddle_->getPos(),
//					rightPaddle_->getWidth(), rightPaddle_->getHeight(),
//					ball_->getPos(), ball_->getWidth(), ball_->getHeight())) {
//
//		// change the direction of the ball, and increment the speed
//		auto &vel = ball_->getVel(); // the use of & is important, so the changes goes directly to the ball
//		vel.setX(-vel.getX());
//		vel = vel * 1.2f;
//
//		// play some sound
//		sdlutils().soundEffects().at("paddle_hit").play();
//	} else if (ball_->getPos().getX() < 0)
//		gm_->onBallExit(GameManager::LEFT);
//	else if (ball_->getPos().getX() + ball_->getWidth() > sdlutils().width())
//		gm_->onBallExit(GameManager::RIGHT);
}

