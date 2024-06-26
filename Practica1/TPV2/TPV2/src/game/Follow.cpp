﻿#include "Follow.h"

#include "../ecs/Manager.h"

Follow::Follow(Vector2D& dest)
	: trans_(nullptr), destination_(dest)
{
}


void Follow::initComponent()
{
	trans_ = mngr_->getComponent<Transform>(ent_);
	assert(trans_ != nullptr);
}

void Follow::update()
{
	auto& pos = trans_->getPos();
	auto& vel = trans_->getVel();
	auto diff = destination_ - pos;
	trans_->setRotation(Vector2D(0, -1).angle(vel));
	vel = vel.rotate(vel.angle(diff) > 0 ? 1.0f : -1.0f);
}
