#pragma once
#include <SDL_stdinc.h>
#include "../Transform.h"
#include "../../ecs/Component.h"
class Vector2D;

constexpr unsigned int MIN_DURATION = 500,
					   MAX_DURATION = 1000;

struct Follow : public ecs::Component
{
	Transform* trans_;
	Vector2D destination_;
	Uint32 timer = 0;
	Uint32 followDuration = MIN_DURATION;
public:
	__CMPID_DECL__(ecs::cmp::FOLLOW)
	Follow(Vector2D&);
	~Follow() override = default;
	void initComponent() override;
	void update() override;
};
