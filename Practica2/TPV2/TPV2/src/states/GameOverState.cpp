#include "GameOverState.h"

#include "../components/p1/HealthComponent.h"
#include "../ecs/Manager.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"
#include "../game/Game.h"

GameOverState::GameOverState()
	: message_(nullptr), rect_()
{
}

GameOverState::~GameOverState()
{
}

void GameOverState::enter()
{
	std::string game = "game_over";
	const auto pacman = getManager()->getHandler(ecs::hdlr::PACMAN);
	if (getManager()->getComponent<HealthComponent>(pacman)->lives_ > 0)
	{	
		game = "game_won";
		sdlutils().soundEffects().at("won").play();
	}
	message_ = &sdlutils().msgs().at(game);
	rect_ = {
		(sdlutils().width() - message_->width()) / 2,
		(sdlutils().height() - message_->height()) / 2,
		message_->width(),
		message_->height()
	};
}

void GameOverState::update()
{
	message_->render(rect_);

	if (ih().keyDownEvent() && ih().isKeyDown(SDL_SCANCODE_RETURN))
	{
		Game::instance()->getManager()->send(
			Message { _m_NEW_GAME }, 
			true
		);
		Game::instance()->setState(NEWGAME);
	}
}

void GameOverState::leave()
{
}
