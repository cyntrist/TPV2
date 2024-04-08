#pragma once
#include <SDL_stdinc.h>
#include "../Transform.h"
#include "../../ecs/Component.h"
class Vector2D;

struct Follow : public ecs::Component
{
	Transform* trans_;
	Vector2D destination_;

public:
	__CMPID_DECL__(ecs::cmp::FOLLOW)
	Follow(Vector2D&);
	~Follow() override = default;
	void initComponent() override;
	void update() override;
	void invertSpeed();
};
