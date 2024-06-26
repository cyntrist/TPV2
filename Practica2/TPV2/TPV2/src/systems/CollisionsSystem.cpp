// This file is part of the course TPV2@UCM - Samir Genaim

#include "CollisionsSystem.h"

#include "../components/Transform.h"
#include "../ecs/Manager.h"
#include "../utils/Collisions.h"
#include "FruitsSystem.h"

CollisionsSystem::CollisionsSystem()
{
	// TODO Auto-generated constructor stub
}

CollisionsSystem::~CollisionsSystem()
{
	// TODO Auto-generated destructor stub
}

void CollisionsSystem::initSystem()
{
}

void CollisionsSystem::update()
{
	// the PacMan's Transform
	//
	auto pm = mngr_->getHandler(ecs::hdlr::PACMAN);
	auto pTR = mngr_->getComponent<Transform>(pm);

	// For safety, we traverse with a normal loop until the current size. In this
	// particular case we could use a for-each loop since the list stars is not
	// modified.
	//
	auto& stars = mngr_->getEntities(ecs::grp::FRUITS);
	auto n = stars.size();
	for (auto i = 0u; i < n; i++)
	{
		auto e = stars[i];
		if (mngr_->isAlive(e))
		{
			// if the star is active (it might have died in this frame)

			// the Star's Transform
			//
			auto eTR = mngr_->getComponent<Transform>(e);

			// check if PacMan collides with the Star (i.e., eat it)
			if (Collisions::collides( //
				pTR->pos_, pTR->width_, pTR->height_, //
				eTR->pos_, eTR->width_, eTR->height_))
			{
				Message m;
				m.id = _m_FRUIT_EATEN;
				m.entity_collided_data.e = e;
				mngr_->send(m);
			}
		}
	}

	auto& ghosts = mngr_->getEntities(ecs::grp::GHOSTS);
	auto gn = ghosts.size();
	for (auto i = 0u; i < gn; i++)
	{
		auto e = ghosts[i];
		if (mngr_->isAlive(e))
		{
			// if the star is active (it might have died in this frame)

			// the Star's Transform
			//
			auto eTR = mngr_->getComponent<Transform>(e);

			// check if PacMan collides with the Ghost 
			if (Collisions::collides( //
				pTR->pos_, pTR->width_, pTR->height_, //
				eTR->pos_, eTR->width_, eTR->height_))
			{
				Message m{_m_PACMAN_GHOST_COLLISION, e};
				mngr_->send(m);
			}
		}
	}
}
