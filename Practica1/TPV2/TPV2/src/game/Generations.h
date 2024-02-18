#pragma once
#include "../ecs/Component.h"

class Generations : public ecs::Component
{
	int generation_;

public:
	__CMPID_DECL__(ecs::cmp::GENERATIONS)

	Generations() : generation_(0)
	{
	}

	Generations(int gen) : generation_(gen)
	{
	}

	int getGeneration() const { return generation_; }
	void setGeneration(int value) { generation_ = value; }
};
