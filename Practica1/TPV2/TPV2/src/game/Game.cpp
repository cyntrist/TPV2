// This file is part of the course TPV2@UCM - Samir Genaim

#include "Game.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Collisions.h"
#include "../ecs/Entity.h"
#include "EmptyRectangleRenderer.h"
#include "GameCtrl.h"
#include "ImageRenderer.h"
#include "InfoMsgs.h"
#include "RectangleRenderer.h"
#include "ScoreRenderer.h"
#include "Transform.h"
#include "DeAcceleration.h"
#include "FighterCtrl.h"
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


// Create the manager
	mngr_ = new ecs::Manager();
	//mngr_->setHandler(ecs::hdlr::PACMAN, pacman);
	fighter_ = mngr_->addEntity();

	auto tr = mngr_->addComponent<Transform>(fighter_);
	tr->initComponent();

	objs_.push_back(fighter_);
	fighter_->setWidth(50.0f);
	fighter_->setHeight(50.0f);
	fighter_->setRotation(90.0);
	fighter_->getPos().set(sdlutils().width() / 2 - fighter_->getWidth()/2,
			sdlutils().height() / 2 - fighter_->getHeight()/2);
	fighter_->addComponent(new Transform());
	fighter_->addComponent(new ImageRenderer(&sdlutils().images().at("fighter")));
	fighter_->addComponent(new FighterCtrl());
	fighter_->addComponent(new DeAcceleration());
	fighter_->addComponent(new ShowAtOppositeSide());
	fighter_->addComponent(new HealthComponent(&sdlutils().images().at("heart"), 3));
	//fighter_->addComponent(new Gun(&sdlutils().images().at("heart"));
//	// game manager
//	gm_ = new GameManager(ball_);
//	gm_->addComponent(new GameCtrl());
//	gm_->addComponent(new ScoreRenderer());
//	gm_->addComponent(new InfoMsgs());
//	objs_.push_back(gm_);

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

		for (auto &o : objs_) {
			o->handleInput();
		}

		// update
		for (auto &o : objs_) {
			o->update();
		}

		//checkCollisions();

		sdlutils().clearRenderer();

		// render
		for (auto &o : objs_) {
			o->render();
		}

		sdlutils().presentRenderer();
		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 20)
			SDL_Delay(20 - frameTime);
	}

}

//void Game::checkCollisions() {
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
//}

