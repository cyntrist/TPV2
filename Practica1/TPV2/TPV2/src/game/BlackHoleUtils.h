#pragma once
#include "BlackHoleFacade.h"
#include "../ecs/ecs.h"
#include "../sdlutils/RandomNumberGenerator.h"

class BlackHoleUtils : public BlackHoleFacade
{
	ecs::Manager* mngr_;
	RandomNumberGenerator& random_;

public:
	BlackHoleUtils();
	~BlackHoleUtils() override = default;
	void create_holes(int n) override;
	void remove_all_holes() override;
};
