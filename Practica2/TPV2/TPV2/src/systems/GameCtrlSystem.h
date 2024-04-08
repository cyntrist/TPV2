// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/System.h"

class GameCtrlSystem : public ecs::System
{
public:
	__SYSID_DECL__(ecs::sys::GAMECTRL)

	GameCtrlSystem();
	~GameCtrlSystem() override;

	auto getScore()
	{
		return score_;
	}

	void setScore(unsigned int score)
	{
		score_ = score;
	}

	void incrScore(unsigned int n)
	{
		score_ += n;
	}

	void initSystem() override;
	void update() override;
	void recieve(const Message& m) override;

private:
	unsigned int score_;
};
