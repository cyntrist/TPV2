// This file is part of the course TPV2@UCM - Samir Genaim

#include "Game.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Collisions.h"
#include "../ecs/Entity.h"
#include "AsteroidsUtils.h"
#include "FighterUtils.h"
#include "Gun.h"
#include "HealthComponent.h"

constexpr float ASTEROID_TIMER = 10000; 

Game::Game()
	: mngr_(nullptr)
{}

Game::~Game() {
	delete mngr_;
	delete f_utils_;
	delete a_utils_;
}

void Game::init() {
	// initialize the SDL singleton
	SDLUtils::init("Asteroids", 800, 600,
			"resources/config/asteroid.resources.json");

	// Manager
	mngr_ = new ecs::Manager();
	// Fighter
	f_utils_ = new FighterUtils();
	f_utils_->create_fighter();
	f_utils_->reset_fighter();
	//Asteroids
	a_utils_ = new AsteroidsUtils();
	a_utils_->create_asteroids(3);
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

		if (sdlutils().currRealTime() > last_asteroid_ + ASTEROID_TIMER)
		{
			a_utils_->create_asteroids(1);
			last_asteroid_ = sdlutils().currRealTime();
		}

		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 10)
			SDL_Delay(10 - frameTime);
	}
}

void Game::checkCollisions() {
	auto fighter = mngr_->getHandler(ecs::hdlr::FIGHTER);
	auto asteroids = mngr_->getEntities(ecs::grp::ASTEROID);

	auto f_trans = mngr_->getComponent<Transform>(fighter);
	auto f_health = mngr_->getComponent<HealthComponent>(fighter);
	// asteroids + fighter
	for( auto a : asteroids)	
	{
		auto a_trans = mngr_->getComponent<Transform>(a);
		if (Collisions::collidesWithRotation(
			f_trans->getPos(), 
			f_trans->getWidth(), 
			f_trans->getHeight(), 
			f_trans->getRot(),
			a_trans->getPos(),
			a_trans->getWidth(),
			a_trans->getHeight(),
			a_trans->getRot()
			))
		{
			f_health->addLives(-1);
			//break;
		}

		// asteroids + bullet
		auto f_gun = mngr_->getComponent<Gun>(fighter);
		for (Gun::Bullet& b : *f_gun)
			if (b.used && Collisions::collidesWithRotation(
			b.pos, 
			b.width, 
			b.height, 
			b.rot,
			a_trans->getPos(),
			a_trans->getWidth(),
			a_trans->getHeight(),
			a_trans->getRot()
			))
			{
				//
				a_utils_->split_astroid(a);
				b.used = false;
				sdlutils().soundEffects().at("explosion").play();
			}
	}
}

