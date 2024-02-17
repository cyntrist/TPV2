#pragma once
#include "FighterFacade.h"

namespace ecs
{
	struct Entity;
	class Manager;
}

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
