#include "GhostSystem.h"

#include "../components/Immune.h"
#include "../components/p1/Follow.h"
#include "../components/p1/HealthComponent.h"
#include "../components/p1/ImageWithFrames.h"
#include "../ecs/Manager.h"
#include "../sdlutils/SDLUtils.h"
#include "../game/Game.h"
struct Immune;
struct Follow;
struct Transform;
constexpr int GHOST_SIZE = 50,
              GHOST_LIMIT = 10;
constexpr Uint32 MIN_SPAWN_TIME = 50,
                 MAX_SPAWN_TIME = 5000;

GhostSystem::GhostSystem()
{
	timer = sdlutils().virtualTimer().currTime();
}

void GhostSystem::addGhost()
{
	const int chance = sdlutils().rand().nextInt(0, 4);
	int x = 0, // esq sup izq
	    y = 0;
	int vx = 1;
	int vy = 1;

	switch (chance)
	{
	case 0: break;
	case 1: // sup der 
		{
			x = sdlutils().width() - GHOST_SIZE / 2;
			vx *= -1;
			break;
		}
	case 2: // inf der
		{
			x = sdlutils().width() - GHOST_SIZE / 2;
			y = sdlutils().height() - GHOST_SIZE / 2;
			vx *= -1;
			vy *= -1;
			break;
		}
	default: // inf izq
		{
			y = sdlutils().height() - GHOST_SIZE / 2;
			vy *= -1;
			break;
		}
	}
	auto ghost = mngr_->addEntity(ecs::grp::GHOSTS);
	auto trans = mngr_->addComponent<Transform>(
		ghost,
		Vector2D(x, y),
		Vector2D(vx, vy),
		GHOST_SIZE / 2,
		GHOST_SIZE / 2,
		0.0f
	);

	/// CHANCE DE COLORINES?
	int color = sdlutils().rand().nextInt(0, 4);
	int firstFrame = 32;
	int lastFrame = 39;
	switch (color)
	{
	case 1:
		firstFrame = 40;
		lastFrame = 47;
		break;
	case 2:
		firstFrame = 48;
		lastFrame = 55;
		break;
	case 3:
		firstFrame = 56;
		lastFrame = 63;
		break;
	}
	mngr_->addComponent<ImageWithFrames>(
		ghost,
		&sdlutils().images().at("atlas"),
		8,
		8,
		firstFrame, // first frame
		lastFrame // last frame
	);

	const auto pacman = mngr_->getHandler(ecs::hdlr::PACMAN);
	auto& pacmanTrans = mngr_->getComponent<Transform>(pacman)->pos_;
	assert(pacman != nullptr);
	mngr_->addComponent<Follow>(ghost, pacmanTrans);
	currentGhosts++;
}

void GhostSystem::destroyGhosts()
{
	for (const auto e : mngr_->getEntities(ecs::grp::GHOSTS))
		mngr_->setAlive(e, false);
	timer = sdlutils().virtualTimer().currTime();
	currentGhosts = 0;
}

void GhostSystem::frightenGhosts()
{
	for (auto e : mngr_->getEntities(ecs::grp::GHOSTS))
	{
		auto iwf = mngr_->getComponent<ImageWithFrames>(e);
		if (iwf != nullptr)
		{
			int firstFrame = 30;
			int lastFrame = 31;

			iwf->firstFrame = firstFrame;
			iwf->lastFrame = lastFrame;
			iwf->currentFrame = firstFrame;
		}
	}
}

void GhostSystem::colorGhosts()
{
	for (auto e : mngr_->getEntities(ecs::grp::GHOSTS))
	{
		auto iwf = mngr_->getComponent<ImageWithFrames>(e);
		if (iwf != nullptr)
		{
			/// CHANCE DE COLORINES?
			int color = sdlutils().rand().nextInt(0, 4);
			int firstFrame = 32;
			int lastFrame = 39;
			switch (color)
			{
			case 1:
				firstFrame = 40;
				lastFrame = 47;
				break;
			case 2:
				firstFrame = 48;
				lastFrame = 55;
				break;
			case 3:
				firstFrame = 56;
				lastFrame = 63;
				break;
			}

			iwf->firstFrame = firstFrame;
			iwf->lastFrame = lastFrame;
			iwf->currentFrame = firstFrame;
		}
	}
}

void GhostSystem::invertSpeeds()
{
	for (const auto e : mngr_->getEntities(ecs::grp::GHOSTS))
	{
		const auto fc = mngr_->getComponent<Follow>(e);
		if (fc != nullptr)
			fc->invertSpeed();
	}
}

void GhostSystem::initSystem()
{
}

void GhostSystem::recieve(const Message& message)
{
	switch (message.id)
	{
	case _m_ROUND_OVER:
		destroyGhosts();
		break;
	case _m_NEW_GAME:
		destroyGhosts();
		break;
	case _m_ROUND_START:
		destroyGhosts();
		break;
	case _m_IMMUNITY_START:
		isPacmanImmune = true;
		frightenGhosts();
		invertSpeeds();
		break;
	case _m_IMMUNITY_END:
		isPacmanImmune = false;
		colorGhosts();
		invertSpeeds();
		break;
	case _m_PACMAN_GHOST_COLLISION:
		if (isPacmanImmune)
		{
			mngr_->setAlive(message.entity_collided_data.e, false);
			currentGhosts--;
			sdlutils().soundEffects().at("eat").play(0, 1);
		}
		else
		{
			auto pacman = mngr_->getHandler(ecs::hdlr::PACMAN);
			auto hc = mngr_->getComponent<HealthComponent>(pacman);

			mngr_->send(Message{_m_ROUND_OVER}, true);

			hc->lives_--;
			sdlutils().soundEffects().at("death").play();

			if (hc->getLives() <= 0)
			{
				mngr_->send(Message{_m_NEW_GAME}, true);
				Game::instance()->setState(GAMEOVER);
			}
			else Game::instance()->setState(NEWROUND);
		}
		break;
	default:
		break;
	}
}

void GhostSystem::update()
{
	const auto currTime = sdlutils().currRealTime();
	const auto pacman = mngr_->getHandler(ecs::hdlr::PACMAN);
	const auto immune = mngr_->getComponent<Immune>(pacman)->isImmune_;
	if (!immune && timer + MIN_SPAWN_TIME < currTime && currentGhosts < GHOST_LIMIT)
	{
		addGhost();
		timer = currTime;
	}

	for (const auto e : mngr_->getEntities(ecs::grp::GHOSTS))
	{
		mngr_->getComponent<Follow>(e)->update();
	}
}
