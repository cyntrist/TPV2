#pragma once
#include "AsteroidsFacade.h"
#include "../sdlutils/RandomNumberGenerator.h"

class AsteroidsUtils : public AsteroidsFacade
{
	ecs::Manager* mngr_;
	RandomNumberGenerator& random_;
public:
	AsteroidsUtils();
	~AsteroidsUtils() override = default;
	void create_asteroids(int n) override;
	void remove_all_asteroids() override;
	void split_astroid(ecs::Entity* a) override;
};
