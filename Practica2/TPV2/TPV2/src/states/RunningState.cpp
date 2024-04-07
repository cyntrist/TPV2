#include "RunningState.h"
#include "../game/Game.h"
#include "../utils/Collisions.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"

#include "../systems/PacManSystem.h"
#include "../systems/GhostSystem.h"
#include "../systems/FruitsSystem.h"
#include "../systems/ImmunitySystem.h"
#include "../systems/CollisionsSystem.h"
#include "../systems/RenderSystem.h"


using Random = RandomNumberGenerator;

RunningState::RunningState(PacManSystem* pc, GhostSystem*gh, FruitsSystem*fo,
	ImmunitySystem*im, RenderSystem*r, CollisionsSystem*col)
	: GameState(nullptr), pacmanSys_(pc), ghostSys_(gh), fruitsSys_(fo),
	immunitySys_(im), renderSys_(r), collisionSys_(col)
{
}

RunningState::~RunningState()
{
}

void RunningState::enter()
{
	fruitsSys_->addFruitGrid(2);
}

void RunningState::update()
{
	pacmanSys_->update();
	ghostSys_->update();
	fruitsSys_->update();
	immunitySys_->update();
	collisionSys_->update();
	renderSys_->update();


	if (ih().isKeyDown(SDL_SCANCODE_P))
		Game::instance()->setState(GAMEOVER);
}

void RunningState::leave()
{
}
