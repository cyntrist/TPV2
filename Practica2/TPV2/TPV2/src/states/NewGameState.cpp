#include "NewGameState.h"
#include "../game/Game.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"

NewGameState::NewGameState()
	: message_(nullptr), rect_()
{
}

NewGameState::~NewGameState()
{
}

void NewGameState::enter()
{
	message_ = &sdlutils().msgs().at("start");
	rect_ = {
		(sdlutils().width() - message_->width()) / 2,
		(sdlutils().height() - message_->height()) / 2,
		message_->width(),
		message_->height()
	};
}

void NewGameState::update()
{
	/*sdlutils().clearRenderer();
	message_->render(rect_);
	sdlutils().presentRenderer();

	if (ih().keyDownEvent())
	{
		g().setState(Game::NEWROUND);
	}*/
}

void NewGameState::leave()
{
}
