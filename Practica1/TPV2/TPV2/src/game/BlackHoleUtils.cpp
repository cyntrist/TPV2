#include "BlackHoleUtils.h"

#include "Game.h"
#include "ImageRenderer.h"
#include "RotateComponent.h"
#include "Transform.h"
#include "../ecs/Manager.h"
#include "../sdlutils/SDLUtils.h"

constexpr float	HOLE_SIZE = 50;

BlackHoleUtils::BlackHoleUtils()
	: mngr_(g().getManager()),
	random_(sdlutils().rand())
{
}

void BlackHoleUtils::create_holes(int n)
{
	const double alpha = (360.0 / n) * M_PI / 180.0f;
	const int centerx = sdlutils().width() / 2;
	const int centery = sdlutils().height() / 2;

	for (int i = 0; i < n; i++)
	{
		const int radio = random_.nextInt(100, 301);
		int x = centerx + radio * cos(alpha * i);
		int y = centery + radio * sin(alpha * i);
		x -= HOLE_SIZE/2;
		y -= HOLE_SIZE/2;
		const auto hole = mngr_->addEntity(ecs::grp::HOLES);
		mngr_->addComponent<Transform>(hole,
		                               Vector2D(x, y), // pos
		                               Vector2D(0, 0), // vel
		                               HOLE_SIZE, // w
		                               HOLE_SIZE, // h
		                               0.0f //r
		);
		mngr_->addComponent<ImageRenderer>(hole, &sdlutils().images().at("black_hole"));
		mngr_->addComponent<RotateComponent>(hole, random_.nextInt(5, 11));
	}
}

void BlackHoleUtils::remove_all_holes()
{
	const auto holes = mngr_->getEntities(ecs::grp::HOLES);
	for (auto h : holes)
		mngr_->setAlive(h, false);
	mngr_->refresh();
}
