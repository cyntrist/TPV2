#pragma once
#include "../ecs/Component.h"
#include "../sdlutils/SDLUtils.h"
struct Immune : ecs::Component
{
	__CMPID_DECL__(ecs::cmp::IMMUNE)
	bool isImmune_ = false;
	Uint32 timer_ = 0;


	Immune()
	{
		timer_ = sdlutils().virtualTimer().currTime();
	}
	void setImmune()
	{
		timer_ = sdlutils().virtualTimer().currTime();
		isImmune_ = true;
	}
	void resetImmune()
	{
		timer_ = sdlutils().virtualTimer().currTime();
		isImmune_ = false;
	}
};