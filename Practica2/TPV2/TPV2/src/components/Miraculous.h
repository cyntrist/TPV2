#pragma once
#include "../ecs/Component.h"

struct Miraculous : ecs::Component
{
	__CMPID_DECL__(ecs::cmp::MIRACULOUS)

	Miraculous() {};
	~Miraculous() override {};
	bool isMiraculous = false;
};