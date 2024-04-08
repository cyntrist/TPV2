#include "NewRoundState.h"

#include "../ecs/Manager.h"
#include "../game/Game.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"

NewRoundState::NewRoundState()
	: message_(nullptr), rect_()
{
}

NewRoundState::~NewRoundState()
{
}

void NewRoundState::enter()
{
	message_ = &sdlutils().msgs().at("round");
	rect_ = {
		(sdlutils().width() - message_->width()) / 2,
		(sdlutils().height() - message_->height()) / 2,
		message_->width(),
		message_->height()
	};
}

void NewRoundState::update()
{
	message_->render(rect_);

	if (ih().keyDownEvent() && ih().isKeyDown(SDL_SCANCODE_RETURN))
	{
		Game::instance()->setState(RUNNING);
		getManager()->send(Message{_m_ROUND_START}, true);
	}
}

void NewRoundState::leave()
{
}
