#include "AsteroidsUtils.h"
#include "Game.h"
#include "../sdlutils/SDLUtils.h"

AsteroidsUtils::AsteroidsUtils()
	: mngr_(g().getManager()),
	random_(sdlutils().rand())
{
}

void AsteroidsUtils::create_asteroids(int n)
{
	
}

void AsteroidsUtils::remove_all_asteroids()
{
}

void AsteroidsUtils::split_astroid(ecs::Entity* a)
{
}
