#pragma once
#include "../ecs/Component.h"

class ShowAtOppositeSide : public ecs::Component
{
public:
	ShowAtOppositeSide() = default;
	~ShowAtOppositeSide() override = default;
	void update() override;
};
