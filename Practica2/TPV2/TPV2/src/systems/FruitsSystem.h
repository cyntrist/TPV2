// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/System.h"

struct ImageWithFrames;

class FruitsSystem : public ecs::System
{
public:
	__SYSID_DECL__(ecs::sys::FRUITS)

	FruitsSystem();
	~FruitsSystem() override;
	void initSystem() override;
	void update() override;
	void addFruitGrid(unsigned int n);
	void onFruitEaten(ecs::entity_t e);
	void changeFruitSprite(ImageWithFrames*, int);
	void resetFruits();
	void destroyFruits();
	void recieve(const Message& m) override;

private:
	unsigned int gridSide;
	unsigned int currNumOfFruits_;
};
