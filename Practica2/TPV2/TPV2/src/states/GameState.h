#pragma once

namespace ecs
{
	class Manager;
}

class GameState
{
	ecs::Manager* mngr_ = nullptr;
public:
	GameState()
	{
	}

	GameState(ecs::Manager* mngr) : mngr_(mngr)
	{
	}

	virtual ~GameState()
	{
	}

	virtual void enter() = 0;
	virtual void leave() = 0;
	virtual void update() = 0;
	void setContext(ecs::Manager* manager) { mngr_ = manager; }
};
