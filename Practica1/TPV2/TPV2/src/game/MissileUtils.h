#pragma once
#include "MissileFacade.h"
#include "../ecs/ecs.h"
#include "../sdlutils/RandomNumberGenerator.h"

constexpr float SIZE = 50;

class MissileUtils : MissileFacade
{
	ecs::Manager* mngr_;
	RandomNumberGenerator& random_;

public:
	MissileUtils();
	~MissileUtils() override = default;
	void create_missiles(int n) override;
	void remove_all_missiles() override;
};
