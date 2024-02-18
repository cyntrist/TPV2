#pragma once
#include "Transform.h"
#include "../ecs/Component.h"
#include "../ecs/Manager.h"

class ShowAtOppositeSide : public ecs::Component
{
	Transform* transform_;

public:
	__CMPID_DECL__(ecs::cmp::OPPOSITE)

	ShowAtOppositeSide();
	~ShowAtOppositeSide() override = default;
	void initComponent() override;
	void update() override;
};
