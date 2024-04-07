#pragma once
#include "../ecs/Component.h"

constexpr Uint32 MIN_TIMER = 1,
				 MAX_TIMER = 2,
				 MIN_DURATION = 5000,
				 MAX_DURATION = 10000;

struct Miraculous : ecs::Component
{
	__CMPID_DECL__(ecs::cmp::MIRACULOUS)

	bool isMiraculous_ = false;
	Uint32 miraculousDuration_ = 0;
	Uint32 nonMiraculousDuration_ = 0;
	Uint32 timer_ = 0;

	Miraculous()
	{
		miraculousDuration_ = sdlutils().rand().nextInt(MIN_DURATION, MAX_DURATION);
		nonMiraculousDuration_ = sdlutils().rand().nextInt(MIN_TIMER, MAX_TIMER);
	}
	~Miraculous() override {};
	void resetMiraculous()
	{
		timer_ = sdlutils().virtualTimer().currTime();	
		isMiraculous_ = false;
	}
	void setMiraculous()
	{
		timer_ = sdlutils().virtualTimer().currTime();
		isMiraculous_ = true;
	}
};