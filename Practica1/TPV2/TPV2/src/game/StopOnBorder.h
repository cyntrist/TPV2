// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/Component.h"

class StopOnBorder: public ecs::Component {
public:
	StopOnBorder();
	virtual ~StopOnBorder();
	void update() override;
};

