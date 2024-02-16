#pragma once
#include "Transform.h"
#include "../ecs/Component.h"

class DeAcceleration : public ecs::Component
{
	Transform* transform_;
public:
	__CMPID_DECL__(ecs::cmp::DEACC)
	DeAcceleration() = default;
	~DeAcceleration() override = default;
	void initComponent() override;
	virtual void update() override;
};
