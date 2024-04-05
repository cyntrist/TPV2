#pragma once
#include "../ecs/System.h"

class ImmunitySystem : public ecs::System
{
public:
	__SYSID_DECL__(ecs::sys::IMMUNITY)
};
