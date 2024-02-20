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

using Random = RandomNumberGenerator;

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
	const auto fighter = mngr_->getHandler(ecs::hdlr::FIGHTER);
	const auto asteroids = mngr_->getEntities(ecs::grp::ASTEROID);
	const auto holes = mngr_->getEntities(ecs::grp::HOLES);
	const auto missiles = mngr_->getEntities(ecs::grp::MISSILES);
	const auto f_trans = mngr_->getComponent<Transform>(fighter);

	// Collisiones de los asteroides
	for (int i = 0; i < asteroids.size(); i++)
	{
		const auto a = asteroids[i];
		const auto a_trans = mngr_->getComponent<Transform>(a);
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
				a_utils_->split_asteroid(a);
				b.used = false;
				sdlutils().soundEffects().at("explosion").play();
			}

		// ASTEROIDS + BLACK HOLES
		for (auto h : holes)
		{
			auto h_trans = mngr_->getComponent<Transform>(h);
			if (Collisions::collidesWithRotation(
				h_trans->getPos(),
				h_trans->getWidth(),
				h_trans->getHeight(),
				h_trans->getRot(),
				a_trans->getPos(),
				a_trans->getWidth(),
				a_trans->getHeight(),
				a_trans->getRot()
			))
			{
				a_utils_->displace_asteroid(a);
				break;
			}
		}
	}

	// Colisiones de los black holes
	for (auto h : holes)
	{
		// holes + fighter
		auto h_trans = mngr_->getComponent<Transform>(h);
		if (Collisions::collidesWithRotation(
			f_trans->getPos(),
			f_trans->getWidth(),
			f_trans->getHeight(),
			f_trans->getRot(),
			h_trans->getPos(),
			h_trans->getWidth(),
			h_trans->getHeight(),
			h_trans->getRot()
		))
		{
			onDeath();
			break;
		}
	}

	// Colisiones de los misiles
	for (auto m : missiles)
	{
		// misiles + caza
		auto m_trans = mngr_->getComponent<Transform>(m);
		if (Collisions::collidesWithRotation(
			f_trans->getPos(),
			f_trans->getWidth(),
			f_trans->getHeight(),
			f_trans->getRot(),
			m_trans->getPos(),
			m_trans->getWidth(),
			m_trans->getHeight(),
			m_trans->getRot()
		))
		{
			onDeath();
			break;
		}

		// misiles + balas
		///
		///
		///
		///
		///
		///
		// misiles + out of bounds
		if (!Collisions::collidesWithRotation(
			Vector2D(0,0),
			sdlutils().width(),
			sdlutils().height(),
			0.0f,
			m_trans->getPos(),
			m_trans->getWidth(),
			m_trans->getHeight(),
			m_trans->getRot()
		))
		{
			mngr_->setAlive(m, false);
			break;
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
