// This file is part of the course TPV2@UCM - Samir Genaim

#include "Transform.h"

#include "../sdlutils/SDLUtils.h"
#include "Container.h"

constexpr double SPEED_LIMIT = 3.0f;	

Transform::Transform() {

}

Transform::~Transform() {
}

void Transform::init(Vector2D pos, Vector2D vel, float w, float h, float r)
{
	pos_ = pos;
	vel_ = vel;
	width_ = w;
	height_ = h;
	rot_ = r;
}

void Transform::update(Container *o) {

	auto &pos = o->getPos();
	auto &vel = o->getVel();

	pos = pos + vel;

	if (abs(vel.getX()) > SPEED_LIMIT || abs(vel.getY()) > SPEED_LIMIT)
	{
		vel = vel.normalize()*SPEED_LIMIT;
	}
}
