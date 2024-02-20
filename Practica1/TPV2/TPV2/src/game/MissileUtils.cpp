#include "MissileUtils.h"

#include "Follow.h"
#include "Game.h"
#include "ImageRenderer.h"
#include "Transform.h"
#include "../ecs/Manager.h"
#include "../sdlutils/SDLUtils.h"

static constexpr float MISS_WIDTH = 50,
                       MISS_HEIGHT = 25;

MissileUtils::MissileUtils()
	: mngr_(g().getManager()),
	  random_(sdlutils().rand())
{
}

void MissileUtils::create_missiles(int n)
{
	for (int i = 0; i < n; i++)
	{
		const int chance = random_.nextInt(0, 4);
		int x = 0, // esq sup izq
		    y = 0;
		int vx = random_.nextInt(1, 4);
		int vy = vx;

		switch (chance)
		{
		case 0: break;
		case 1: // sup der 
			{
				x = sdlutils().width() - MISS_WIDTH;
				vx *= -1;
				break;
			}
		case 2: // inf der
			{
				x = sdlutils().width() - MISS_WIDTH;
				y = sdlutils().height() - MISS_HEIGHT;
				vx *= -1;
				vy *= -1;
				break;
			}
		default: // inf izq
			{
				y = sdlutils().height() - MISS_HEIGHT;
				vy *= -1;
				break;
			}
		}
		auto missile = mngr_->addEntity(ecs::grp::MISSILES);
		auto trans = mngr_->addComponent<Transform>(missile,
		                               Vector2D(x, y),
		                               Vector2D(vx, vy),
		                               MISS_WIDTH,
		                               MISS_HEIGHT,
		                               0.0f
		);
		mngr_->addComponent<ImageRenderer>(missile, &sdlutils().images().at("missile"));
		const auto fighter = mngr_->getHandler(ecs::hdlr::FIGHTER);
		auto& f_pos = mngr_->getComponent<Transform>(fighter)->getPos();
		mngr_->addComponent<Follow>(missile, f_pos);
		std::cout << trans->getRot() << std::endl;

	}
}

void MissileUtils::remove_all_missiles()
{
	const auto missiles = mngr_->getEntities(ecs::grp::MISSILES);
	for (auto m : missiles)
		mngr_->setAlive(m, false);
	mngr_->refresh();
}
