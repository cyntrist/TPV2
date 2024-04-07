#pragma once
#include <SDL_stdinc.h>

#include "../ecs/System.h"

class Vector2D;

class GhostSystem : public ecs::System
{
	Uint32 timer = 0;
public:
	__SYSID_DECL__(ecs::sys::GHOST)
	GhostSystem();
	void addGhost();
	void destroyGhosts();

	void initSystem() override;
	void recieve(const Message&) override;
	void update() override;
};
