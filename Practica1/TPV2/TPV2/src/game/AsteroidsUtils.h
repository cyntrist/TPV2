#pragma once
#include "AsteroidsFacade.h"
#include "../sdlutils/RandomNumberGenerator.h"
#include "../utils/Vector2D.h"

class AsteroidsUtils : public AsteroidsFacade
{
	ecs::Manager* mngr_;
	RandomNumberGenerator& random_;
	void create_asteroid(Vector2D, Vector2D, int);

public:
	AsteroidsUtils();
	~AsteroidsUtils() override = default;
	void create_asteroids(int n) override;
	void remove_all_asteroids() override;
	void split_astroid(ecs::entity_t a) override;
};
