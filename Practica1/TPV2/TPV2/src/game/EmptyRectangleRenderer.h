// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <SDL.h>
#include "../ecs/Component.h"

class EmptyRectangleRenderer: public ecs::Component {
public:
	EmptyRectangleRenderer();
	EmptyRectangleRenderer(SDL_Color color);
	virtual ~EmptyRectangleRenderer();
	void render() override;
private:
	SDL_Color color_;
};

