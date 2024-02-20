#include "RunningState.h"
#include "Game.h"
#include "../utils/Collisions.h"
#include "Gun.h"
#include "HealthComponent.h"
#include "Transform.h"
#include "../sdlutils/SDLUtils.h"
#include "AsteroidsUtils.h"
#include "FighterUtils.h"
#include "../sdlutils/InputHandler.h"

constexpr float ASTEROID_TIMER = 5000;
constexpr float MISSILE_TIMER = 15000;

RunningState::RunningState(FighterUtils* fu, AsteroidsUtils* au,
                           BlackHoleUtils* bhu, MissileUtils* mu)
	: mngr_(nullptr), f_utils_(fu), a_utils_(au),
	  bh_utils_(bhu), m_utils_(mu), last_asteroid_(0)
{
}

RunningState::~RunningState()
{
}

void RunningState::enter()
{
	mngr_ = g().getManager();
}

void RunningState::update()
{
	if (mngr_->getEntities(ecs::grp::ASTEROID).empty())
	{
		g().setState(Game::GAMEOVER);
		return;
	}

	if (ih().isKeyDown(SDL_SCANCODE_P))
	{
		g().setState(Game::PAUSED);
		return;
	}

	mngr_->update();
	mngr_->refresh();

	checkCollisions();

	sdlutils().clearRenderer();
	mngr_->render();
	sdlutils().presentRenderer();

	const int now = sdlutils().virtualTimer().currTime();

	if (now > last_asteroid_ + ASTEROID_TIMER)
	{
		a_utils_->create_asteroids(1);
		last_asteroid_ = now;
	}

	if (now > last_missile_ + MISSILE_TIMER)
	{
		m_utils_->create_missiles(1);
		last_missile_ = now;
	}
}

void RunningState::leave()
{
}

void RunningState::checkCollisions()
{
	auto fighter = mngr_->getHandler(ecs::hdlr::FIGHTER);
	auto asteroids = mngr_->getEntities(ecs::grp::ASTEROID);
	auto f_trans = mngr_->getComponent<Transform>(fighter);

	for (int i = 0; i < asteroids.size(); i++)
	{
		auto a = asteroids[i];
		auto a_trans = mngr_->getComponent<Transform>(a);
		if (Collisions::collidesWithRotation(
			f_trans->getPos(),
			f_trans->getWidth(),
			f_trans->getHeight(),
			f_trans->getRot(),
			a_trans->getPos(),
			a_trans->getWidth(),
			a_trans->getHeight(),
			a_trans->getRot()
		))
		{
			onDeath();
			break;
		}

		// asteroids + bullet
		auto f_gun = mngr_->getComponent<Gun>(fighter);
		for (Gun::Bullet& b : *f_gun)
			if (b.used && Collisions::collidesWithRotation(
				b.pos,
				b.width,
				b.height,
				b.rot,
				a_trans->getPos(),
				a_trans->getWidth(),
				a_trans->getHeight(),
				a_trans->getRot()
			))
			{
				a_utils_->split_astroid(a);
				b.used = false;
				sdlutils().soundEffects().at("explosion").play();
			}
	}
}

void RunningState::onDeath()
{
	sdlutils().soundEffects().at("bang").play();
	if (f_utils_->update_lives(-1) > 0)
		g().setState(Game::NEWROUND);
	else
		g().setState(Game::GAMEOVER);
}
