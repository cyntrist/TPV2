#pragma once
#include "../ecs/Component.h"

class Follow : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::cmp::FOLLOWS)
};
