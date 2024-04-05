// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/System.h"

class FruitsSystem: public ecs::System {
public:

	__SYSID_DECL__(ecs::sys::FRUITS)

	FruitsSystem();
	virtual ~FruitsSystem();
	void initSystem() override;
	void update() override;
	void addStar(unsigned int n);
	void onStarEaten(ecs::entity_t e);
	void recieve(const Message &m) override;
private:
	unsigned int starsLimit_;
	unsigned int currNumOfStars_;
};

