#include "NewRoundState.h"
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
	/*message_ = &sdlutils().msgs().at("round");
	rect_ = {
		(sdlutils().width() - message_->width()) / 2,
		(sdlutils().height() - message_->height()) / 2,
		message_->width(),
		message_->height()
	};*/
}

void NewRoundState::update()
{
	/*sdlutils().clearRenderer();
	message_->render(rect_);
	sdlutils().presentRenderer();

	if (ih().keyDownEvent() && ih().isKeyDown(SDL_SCANCODE_RETURN))
	{
		f_utils_->reset_fighter();

		a_utils_->remove_all_asteroids();
		a_utils_->create_asteroids(ASTEROID_WAVE);

		bh_utils_->remove_all_holes();
		bh_utils_->create_holes(HOLE_WAVE);

		m_utils_->remove_all_missiles();

		g().setState(Game::RUNNING);
	}*/
}

void NewRoundState::leave()
{
}
