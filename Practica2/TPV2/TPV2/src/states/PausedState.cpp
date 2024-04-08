#include "PausedState.h"
#include "../game/Game.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"

PausedState::PausedState()
	: message_(nullptr), rect_()
{
}

PausedState::~PausedState()
{
}

void PausedState::enter()
{
	message_ = &sdlutils().msgs().at("pause");
	rect_ = {
		(sdlutils().width() - message_->width()) / 2,
		(sdlutils().height() - message_->height()) / 2,
		message_->width(),
		message_->height()
	};

	sdlutils().virtualTimer().pause();
}

void PausedState::update()
{
	message_->render(rect_);

	if (ih().keyDownEvent()) 
		Game::instance()->setState(RUNNING);
}

void PausedState::leave()
{
	sdlutils().virtualTimer().resume();
}
