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
	ecs::Entity* fighter_;
public:
	FighterUtils();
	~FighterUtils() override = default;
	void create_fighter() override;
	void reset_fighter() override;
	void reset_lives() override;
	int update_lives(int n) override;
};
