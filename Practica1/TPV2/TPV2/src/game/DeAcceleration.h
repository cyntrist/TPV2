#pragma once
#include "PhysicsComponent.h"

class DeAcceleration : public PhysicsComponent
{
public:
	DeAcceleration() = default;
	~DeAcceleration() override = default;
	virtual void update(Container *o) override;
};