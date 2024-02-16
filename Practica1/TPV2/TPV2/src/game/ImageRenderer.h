// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include "../ecs/Component.h"

class Texture;

class ImageRenderer: public ecs::Component {
public:
	ImageRenderer(Texture *img);
	virtual ~ImageRenderer();
	void render() override;
private:
	Texture *img_;
};

