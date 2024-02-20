#include "RotateComponent.h"

#include "Game.h"
#include "Transform.h"

RotateComponent::RotateComponent() : rot_amount_(1)
{
}

RotateComponent::RotateComponent(float rot) : rot_amount_(rot)
{
}

void RotateComponent::initComponent()
{
	trans_ = mngr_->getComponent<Transform>(ent_);
	assert(trans_ != nullptr);
}

void RotateComponent::update()
{
	trans_->setRotation(trans_->getRot() + rot_amount_);
}
