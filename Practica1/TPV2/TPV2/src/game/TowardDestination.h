#pragma once
#include "Transform.h"
#include "../ecs/Component.h"
#include "../sdlutils/RandomNumberGenerator.h"

class TowardDestination : public ecs::Component
{
	Transform* trans_;
	Vector2D destination_;
	RandomNumberGenerator& random_;

	void steer();

public:
	__CMPID_DECL__(ecs::cmp::TOWARDSDEST)
	TowardDestination();
	void initComponent() override;
	void update() override;
};
