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
	__CMPID_DECL__(ecs::cmp::TRANSFORM)

	Transform();
	virtual ~Transform() override;
	Transform(Vector2D pos, Vector2D vel, float w, float h, float r);
	void init(Vector2D pos, Vector2D vel, float w, float h, float r);
	void update() override;

	Vector2D& getPos() { return pos_; }
	Vector2D& getVel() { return vel_; }
	float getWidth() const { return width_; }
	float getHeight() const { return width_; }
	float getRot() const { return rot_; }

	void setPos(const Vector2D& value) { pos_ = value; }
	void setVel(const Vector2D& value) { vel_ = value; }
	void setWidth(const float value) { width_ = value; }
	void setHeight(const float value) { height_ = value; }
	void setRotation(const float value) { rot_ = value; }
};

