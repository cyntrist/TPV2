#include "PausedState.h"
#include "Game.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"
PausedState::PausedState()
{
}

PausedState::~PausedState()
{
}

void PausedState::enter()
{
	message_ = &sdlutils().msgs().at("pause");
	rect_ = {
		(sdlutils().width() - message_->width())/2,
		(sdlutils().height() - message_->height())/2,
		message_->width(),
		message_->height()
	};

	sdlutils().virtualTimer().pause();
}

void PausedState::update()
{
	sdlutils().clearRenderer();
	message_->render(rect_);
	sdlutils().presentRenderer();

	if (ih().keyDownEvent())
		g().setState(Game::RUNNING);
}

void PausedState::leave()
{
	sdlutils().virtualTimer().reset();
}
