#include "NewGameState.h"
#include "FighterUtils.h"
#include "Game.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"
NewGameState::NewGameState(FighterUtils* fu)
	: f_utils_(fu)
{
}

NewGameState::~NewGameState()
{
}

void NewGameState::enter()
{
	message_ = &sdlutils().msgs().at("start");
	rect_ = {
		(sdlutils().width() - message_->width())/2,
		(sdlutils().height() - message_->height())/2,
		message_->width(),
		message_->height()
	};
}

void NewGameState::update()
{
	sdlutils().clearRenderer();
	message_->render(rect_);
	sdlutils().presentRenderer();

	if (ih().keyDownEvent())
	{
		f_utils_->reset_lives();
		g().setState(Game::NEWROUND);
	}
}

void NewGameState::leave()
{
}
