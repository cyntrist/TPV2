// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/System.h"

class CollisionsSystem : public ecs::System
{
public:
	__SYSID_DECL__(ecs::sys::COLLISIONS)
	bool isPacmanImmune = false;
	CollisionsSystem();
	~CollisionsSystem() override;
	void initSystem() override;
	void update() override;
};
