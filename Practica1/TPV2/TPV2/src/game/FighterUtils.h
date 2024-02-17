#pragma once
#include "FighterFacade.h"

class FighterUtils : public FighterFacade
{
	ecs::Manager* mngr_;
	ecs::Entity* fighter_ = nullptr;
public:
	FighterUtils() = default;
	~FighterUtils() = default;
	void create_fighter() override;
	void reset_fighter() override;
	void reset_lives() override;
	int update_lives(int n) override;
};
