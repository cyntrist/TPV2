#pragma once
#include <SDL_stdinc.h>
#include "../ecs/System.h"

struct Immune;
constexpr Uint32 IMMUNITY_DURATION = 5000;
constexpr Uint8 NORMAL_FIRST_FRAME = 0,
				NORMAL_LAST_FRAME = 3,
				IMMUNE_FIRST_FRAME = 8,
				IMMUNE_LAST_FRAME = 11;

class ImmunitySystem : public ecs::System
{
	ecs::entity_t pacman = nullptr;
	Immune* ic = nullptr;
public:
	__SYSID_DECL__(ecs::sys::IMMUNITY)
	ImmunitySystem();
	void initSystem() override;
	void update() override;
	void recieve(const Message&) override;
	void changePacmanSprite(int first, int last);
	void setImmune();
	void resetImmune();
};
