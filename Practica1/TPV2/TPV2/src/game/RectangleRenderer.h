// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <SDL_pixels.h>
#include "../ecs/Component.h"

class RectangleRenderer: public ecs::Component {
public:
	RectangleRenderer();
	RectangleRenderer(SDL_Color color);
	virtual ~RectangleRenderer();

	virtual void render() override;

private:
	SDL_Color color_;
};

