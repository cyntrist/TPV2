#include "FighterUtils.h"

#include "DeAcceleration.h"
#include "FighterCtrl.h"
#include "../sdlutils/SDLUtils.h"
#include "Game.h"
#include "Gun.h"
#include "HealthComponent.h"
#include "ImageRenderer.h"
#include "ShowAtOppositeSide.h"
#include "Transform.h"

void FighterUtils::create_fighter()
{
	mngr_ = g().getManager();
	auto fighter_ = mngr_->addEntity();

	// transform
	auto tr = mngr_->addComponent<Transform>(fighter_);
	auto size = 50;
	tr->init(
		Vector2D(sdlutils().width()/2, sdlutils().height()/2),
		Vector2D(0,0),
		size,
		size,
		90.0f
	);
	tr->setPos(Vector2D(sdlutils().width() / 2 - tr->getWidth()/2,
			sdlutils().height() / 2 - tr->getHeight()/2));

	// componentes
	mngr_->setHandler(ecs::hdlr::FIGHTER, fighter_);
	mngr_->addComponent<ImageRenderer>(fighter_, &sdlutils().images().at("fighter"));
	mngr_->addComponent<FighterCtrl>(fighter_);
	mngr_->addComponent<DeAcceleration>(fighter_);
	mngr_->addComponent<ShowAtOppositeSide>(fighter_);
	mngr_->addComponent<HealthComponent>(fighter_, &sdlutils().images().at("heart"), 3);
	mngr_->addComponent<Gun>(fighter_, &sdlutils().images().at("fire"));
}

void FighterUtils::reset_fighter()
{
}

void FighterUtils::reset_lives()
{
}

int FighterUtils::update_lives(int n)
{
	return 0;
}
