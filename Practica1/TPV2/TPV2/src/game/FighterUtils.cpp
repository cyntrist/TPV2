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

constexpr uint8_t LIVES = 3;

void FighterUtils::create_fighter()
{
	mngr_ = g().getManager();
	fighter_ = mngr_->addEntity(ecs::grp::FIGHTER);

	// transform
	const auto tr = mngr_->addComponent<Transform>(fighter_);
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
	mngr_->addComponent<HealthComponent>(fighter_, &sdlutils().images().at("heart"), LIVES);
	mngr_->addComponent<Gun>(fighter_, &sdlutils().images().at("fire"));
}

void FighterUtils::reset_fighter()
{
	assert(fighter_ != nullptr);
	const auto& tr = mngr_->getComponent<Transform>(fighter_);
	tr->setPos(Vector2D(sdlutils().width() / 2 - tr->getWidth()/2,
			sdlutils().height() / 2 - tr->getHeight()/2));
	tr->setVel(Vector2D(0,0));
	mngr_->getComponent<Gun>(fighter_)->reset();
}

void FighterUtils::reset_lives()
{
	assert(fighter_ != nullptr);
	mngr_->getComponent<HealthComponent>(fighter_)->resetLives();
}

int FighterUtils::update_lives(int n)
{
	assert(fighter_ != nullptr);
	const auto health = mngr_->getComponent<HealthComponent>(fighter_);
	health->addLives(n);
	return health->getLives(); 
}
