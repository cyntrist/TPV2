// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include "../ecs/Component.h"

class ScoreRenderer: public ecs::Component {
public:
	ScoreRenderer();
	virtual ~ScoreRenderer();
	void render() override;
};

