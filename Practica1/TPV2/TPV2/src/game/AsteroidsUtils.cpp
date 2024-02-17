#include "AsteroidsUtils.h"
#include "Game.h"
#include "Generations.h"
#include "ImageWithFrames.h"
#include "ShowAtOppositeSide.h"
#include "../sdlutils/SDLUtils.h"

AsteroidsUtils::AsteroidsUtils()
	: mngr_(g().getManager()),
	random_(sdlutils().rand())
{
}

void AsteroidsUtils::create_asteroids(int n)
{
	for (int i = 0; i < n; i++)
	{
		auto* asteroid = mngr_->addEntity(ecs::grp::ASTEROID);
		mngr_->addComponent<Transform>(asteroid);
		mngr_->addComponent<ImageWithFrames>(asteroid);
		mngr_->addComponent<ShowAtOppositeSide>(asteroid);
		mngr_->addComponent<Generations>(asteroid);

		// o uno u otro: creo
		//mngr_->addComponent<Follow>(asteroid);	
		//mngr_->addComponent<Follow>(asteroid);	
	}
}

void AsteroidsUtils::remove_all_asteroids()
{
	for (auto a : mngr_->getEntities(ecs::grp::ASTEROID))
		mngr_->setAlive(a, false);
	mngr_->refresh();
}

void AsteroidsUtils::split_astroid(ecs::entity_t * a)
{
	//...
}
