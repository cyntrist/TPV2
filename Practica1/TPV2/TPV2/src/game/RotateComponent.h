#pragma once
#include "../ecs/Component.h"

class RotateComponent : public ecs::Component
{
	float rot_amount_;

public:
	RotateComponent();
	RotateComponent(float);
	void update() override;
};
