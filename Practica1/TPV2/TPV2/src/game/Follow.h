#pragma once
#include "Transform.h"
#include "../ecs/Component.h"

class Follow : public ecs::Component
{
	Transform* trans_;
	Vector2D& destination_;
public:
	__CMPID_DECL__(ecs::cmp::FOLLOWS)
	Follow(Vector2D&);
	~Follow() override = default;
	void initComponent() override;
	void update() override;
};
