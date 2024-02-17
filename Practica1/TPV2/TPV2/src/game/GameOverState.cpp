#include "GameOverState.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"
#include "Game.h"
GameOverState::GameOverState()
{
}

GameOverState::~GameOverState()
{
}

void GameOverState::enter()
{
	if (g().getManager()->getEntities(ecs::grp::ASTEROID).empty())
		message_ = &sdlutils().msgs().at("game_won");
	else
		message_ = &sdlutils().msgs().at("game_over");
	rect_ = {
		sdlutils().width() - message_->width()/2,
		sdlutils().height() - message_->height()/2,
		message_->width(),
		message_->height()
	};
}

void GameOverState::update()
{
	if (ih().isKeyDown(SDL_SCANCODE_RETURN))
		g().setState(Game::NEWGAME);
}

void GameOverState::leave()
{
}
