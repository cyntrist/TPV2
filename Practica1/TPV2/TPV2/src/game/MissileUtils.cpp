#include "MissileUtils.h"

#include "Game.h"
#include "Transform.h"
#include "../ecs/Manager.h"

MissileUtils::MissileUtils()
: mngr_(g().getManager())
{
}

void MissileUtils::create_missiles(int n)
{
	auto missile = mngr_->addEntity(ecs::grp::MISSILES);
	mngr_->addComponent<Transform>(missile);
}

void MissileUtils::remove_all_missiles()
{
	const auto missiles = mngr_->getEntities(ecs::grp::MISSILES);
	for (auto m : missiles)
		mngr_->setAlive(m, false);
	mngr_->refresh();
}
