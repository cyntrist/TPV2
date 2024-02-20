#pragma once
#include "MissileFacade.h"
#include "../ecs/ecs.h"

class MissileUtils : MissileFacade
{
	ecs::Manager* mngr_;

public:
	MissileUtils();
	~MissileUtils() override = default;
	void create_missiles(int n) override;
	void remove_all_missiles() override;
};
