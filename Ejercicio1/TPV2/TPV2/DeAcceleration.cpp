#include "DeAcceleration.h"
#include "src/game/Container.h"

constexpr double FRICTION_FACTOR = 0.995f;

void DeAcceleration::update(Container* o)
{
	auto &vel = o->getVel();
	vel = vel * FRICTION_FACTOR;
}
