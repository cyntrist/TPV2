#include "AsteroidsUtils.h"
#include "Follow.h"
#include "Game.h"
#include "Generations.h"
#include "ImageWithFrames.h"
#include "ShowAtOppositeSide.h"
#include "TowardDestination.h"
#include "../sdlutils/SDLUtils.h"

constexpr int PADDING = 50;
constexpr float MIN_SIZE = 24.0f;
constexpr float SIZE_FACTOR = 5.0f;
constexpr int CHILDREN = 2;

AsteroidsUtils::AsteroidsUtils()
	: mngr_(g().getManager()),
	  random_(sdlutils().rand())
{
}

void AsteroidsUtils::create_asteroids(int n)
{
	for (int i = 0; i < n; i++)
	{
		const int borde = random_.nextInt(0, 4);
		Vector2D pos(PADDING, PADDING);
		switch (borde)
		{
		case 0: // borde superior
			pos.setX(random_.nextInt(PADDING, sdlutils().width() - PADDING));
			break;
		case 1: // borde derecha
			pos.setX(sdlutils().width() - PADDING);
			pos.setY(random_.nextInt(PADDING, sdlutils().height() - PADDING));
			break;
		case 2: // borde abajo
			pos.setY(sdlutils().height() - PADDING);
			pos.setX(random_.nextInt(PADDING, sdlutils().width() - PADDING));
			break;
		case 3: // borde izquierda
			pos.setY(random_.nextInt(PADDING, sdlutils().height() - PADDING));
			break;
		default:
			break;
		}

		const auto gen = random_.nextInt(1, 4);
		const auto size = MIN_SIZE + SIZE_FACTOR * gen;

		pos.setX(pos.getX() - size / 2);
		pos.setY(pos.getY() - size / 2);

		// velocidad aleatoria hacia el centro de la ventana
		auto speed = sdlutils().rand().nextInt(1, 10) / 10.0f;
		auto center = Vector2D(sdlutils().width()/2, sdlutils().height()/2)
			+ Vector2D(random_.nextInt(-100, 100), random_.nextInt(-100, 100));
		auto vel = (center - pos).normalize() * speed;

		create_asteroid(pos, vel, size, gen);
	}
}

void AsteroidsUtils::remove_all_asteroids()
{
	const auto asteroids = mngr_->getEntities(ecs::grp::ASTEROID);
	for (auto a : asteroids)
		mngr_->setAlive(a, false);
	mngr_->refresh();
}

void AsteroidsUtils::split_astroid(ecs::entity_t a)
{
	mngr_->setAlive(a, false);
	auto pos = mngr_->getComponent<Transform>(a)->getPos();
	auto vel = mngr_->getComponent<Transform>(a)->getVel();
	auto width = mngr_->getComponent<Transform>(a)->getWidth();
	auto height = mngr_->getComponent<Transform>(a)->getHeight();
	auto gen = mngr_->getComponent<Generations>(a)->getGeneration();
	if (gen > 0)
		for (int i = 0; i < CHILDREN; i++)
		{
			int r = sdlutils().rand().nextInt(0, 360);
			int side = std::max(width, height);
			Vector2D newPos = pos + vel.rotate(r) * 2 * side;
			Vector2D newVel = vel.rotate(r) * 1.1f;
			create_asteroid(newPos, newVel, side, gen - 1);
		}
}

void AsteroidsUtils::create_asteroid(Vector2D pos, Vector2D vel, int size, int gen)
{
	auto* asteroid = mngr_->addEntity(ecs::grp::ASTEROID);

	std::string type = "asteroid";
	auto transform = mngr_->addComponent<Transform>(asteroid);
	transform->init(pos, vel, size, size, 0.0f);

	int chance = random_.nextInt(0, 4); // 25% probabilidad de hacer un asteroide dorado
	auto fighter = mngr_->getHandler(ecs::hdlr::FIGHTER);
	auto f_trans = mngr_->getComponent<Transform>(fighter);
	switch (chance)
	{
	case 0:
		type = "asteroid_gold";
		mngr_->addComponent<Follow>(asteroid, f_trans->getPos());
		break;
	case 1:
		mngr_->addComponent<TowardDestination>(asteroid);
		break;
	default:
		break;
	}
	mngr_->addComponent<ShowAtOppositeSide>(asteroid);
	mngr_->addComponent<Generations>(asteroid, gen);
	mngr_->addComponent<ImageWithFrames>(asteroid, &sdlutils().images().at(type), 6, 5);
}
