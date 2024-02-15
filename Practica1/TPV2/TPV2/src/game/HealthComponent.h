#pragma once
#include "RenderComponent.h"

class Texture;

class HealthComponent : public RenderComponent
{
	Texture* image_;
	int lives_, iniLives_;

public:
	HealthComponent(Texture *img, int lives);
	~HealthComponent() override = default;

	int getLives() const { return lives_; }
	void subLives() { lives_--; }
	void addLives() { lives_++; }
	void resetLives() { lives_ = iniLives_; }
	void render(Container* o) override;
};
