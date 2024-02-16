// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../utils/Vector2D.h"
#include "../ecs/Component.h"

class Transform: public ecs::Component {
	Vector2D pos_;
	Vector2D vel_;
	float width_;
	float height_;
	float rot_;

public:
	Transform();
	virtual ~Transform();
	void init(Vector2D pos, Vector2D vel, float w, float h, float r);
	void update() override;
};

