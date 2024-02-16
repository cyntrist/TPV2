#pragma once
#include <SDL_scancode.h>
#include "../sdlutils/InputHandler.h"
#include <SDL.h>

#include "Transform.h"
#include "../ecs/Component.h"

class FighterCtrl : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::cmp::FIGHTERCTRL)

	FighterCtrl();
	virtual ~FighterCtrl() override = default;
	void initComponent() override;
	void update() override;

private:
	Transform* transform_;
	SDL_Scancode up_;
	SDL_Scancode left_;
	SDL_Scancode right_;
};
