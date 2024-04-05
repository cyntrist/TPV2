#pragma once
#include "../../ecs/Component.h"

class Texture;

struct HealthComponent : public ecs::Component
{
	Texture* image_;
	int lives_, iniLives_;

public:
	__CMPID_DECL__(ecs::cmp::HEALTH)

	HealthComponent(Texture* img, int lives = 3);
	~HealthComponent() override;

	void initComponent() override;
	int getLives() const { return lives_; }
	int getInitLives() const { return iniLives_; }
	void addLives(int value) { lives_ += value; }
	void setLives(int value) { lives_ = value; }
	void resetLives() { lives_ = iniLives_; }
	void render() override;
};
