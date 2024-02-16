#pragma once
#include "../ecs/Component.h"

class Texture;

class HealthComponent : public ecs::Component
{
	Texture* image_;
	int lives_, iniLives_;

public:
	__CMPID_DECL__(ecs::cmp::HEALTH)

	HealthComponent(Texture *img, int lives);
	~HealthComponent() override = default;

	void initComponent() override;
	int getLives() const { return lives_; }
	void subLives() { lives_--; }
	void addLives() { lives_++; }
	void resetLives() { lives_ = iniLives_; }
	void render() override;
};
