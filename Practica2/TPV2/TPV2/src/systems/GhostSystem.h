﻿#pragma once
#include <SDL_stdinc.h>

#include "../ecs/System.h"

class Vector2D;

class GhostSystem : public ecs::System
{
	Uint32 timer = 0;
	Uint8 currentGhosts = 0;
	bool isPacmanImmune = false;

public:
	__SYSID_DECL__(ecs::sys::GHOST)
	GhostSystem();
	void addGhost();
	void destroyGhosts();
	void frightenGhosts();
	void colorGhosts();
	void invertSpeeds();

	void initSystem() override;
	void recieve(const Message&) override;
	void update() override;
};
