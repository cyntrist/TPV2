// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include "../ecs/System.h"

struct Transform;

class PacManSystem: public ecs::System {
public:

	__SYSID_DECL__(ecs::sys::PACMAN)

	PacManSystem();
	virtual ~PacManSystem();
	void initSystem() override;
	void update() override;
	void recieve(const Message&) override;

	void resetPosition();
	void resetLives();
private:
	Transform *pmTR_;
	ecs::entity_t pacman;
};

