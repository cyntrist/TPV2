#pragma once
#include "src/game/PhysicsComponent.h"

class ShowAtOppositeSide : public PhysicsComponent
{
public:
	ShowAtOppositeSide() = default;
	~ShowAtOppositeSide() override = default;
	void update(Container *o) override;
};
