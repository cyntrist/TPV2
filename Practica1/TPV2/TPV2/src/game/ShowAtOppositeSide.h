#pragma once
#include "Transform.h"
#include "../ecs/Component.h"
#include "../ecs/Manager.h"
class ShowAtOppositeSide : public ecs::Component
{
	Transform* transform_;
public:
	ShowAtOppositeSide() = default;
	~ShowAtOppositeSide() override = default;
	void initComponent() override;
	void update() override;
};
