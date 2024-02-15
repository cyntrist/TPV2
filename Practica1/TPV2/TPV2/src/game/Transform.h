// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include "PhysicsComponent.h"

class Transform: public PhysicsComponent {
public:
	Transform();
	virtual ~Transform();
	void update(Container *o) override;
};

