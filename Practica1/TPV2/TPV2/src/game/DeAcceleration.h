#pragma once
#include "../ecs/Component.h"

class DeAcceleration : public ecs::Component
{
public:
	DeAcceleration() = default;
	~DeAcceleration() override = default;
	virtual void update() override;
};
