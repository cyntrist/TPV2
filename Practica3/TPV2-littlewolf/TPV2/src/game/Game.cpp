// This file is part of the course TPV2@UCM - Samir Genaim

#include "Game.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "LittleWolf.h"
#include "Networking.h"


Game::Game() :
		little_wolf_(nullptr) //
{
}

Game::~Game() {
	delete little_wolf_;
}

bool Game::init(const char *host, Uint16 port) {
	net_ = new Networking();

	if (!net_->init(host, port)) {
		SDLNetUtils::print_SDLNet_error();
	}
	std::cout << "Connected as client " << (int)net_->client_id() << std::endl;

	// initialize the SDLUtils singleton
	SDLUtils::init("Little Wolf Online", 900, 480,
			"resources/config/littlewolf.resources.json");

	little_wolf_ = new LittleWolf(sdlutils().width(), sdlutils().height(),
			sdlutils().window(), sdlutils().renderer());

	// load a map
	little_wolf_->load("resources/maps/little_wolf/map_1.txt");

	// add some players
	little_wolf_->addPlayer(net_->client_id());
	//little_wolf_->addPlayer(1);
	//little_wolf_->addPlayer(2);
	//little_wolf_->addPlayer(3);

	little_wolf_->send_my_info();

	return true;
}

void Game::start() {

	// a boolean to exit the loop
	bool exit = false;

	auto &ihdlr = ih();

	while (!exit) {
		Uint32 startTime = sdlutils().currRealTime();

		// refresh the input handler
		ihdlr.refresh();
		if (ihdlr.keyDownEvent()) {

			// ESC exists the game
			if (ihdlr.isKeyDown(SDL_SCANCODE_ESCAPE)) {
				exit = true;
				continue;
			}

			if (ihdlr.isKeyDown(SDL_SCANCODE_C)) {
				little_wolf_->toggleCenital();
			}
		}

		net_->update();

		little_wolf_->update();

		// the clear is not necessary since we copy the whole texture -- I guess ...
		// sdlutils().clearRenderer();

		little_wolf_->render();

		sdlutils().presentRenderer();


		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 10)
			SDL_Delay(10 - frameTime);
	}
	net_->disconnect();
}

