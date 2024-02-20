// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "GameState.h"
#include "../ecs/Manager.h"
#include "../utils/Singleton.h"
class BlackHoleUtils;
class AsteroidsUtils;
class FighterUtils;
class MissileUtils;

namespace ecs
{
	class Manager;
	struct Entity;
}

class Game : public Singleton<Game>
{
public:
	~Game() override;
	void init();
	void start();
	ecs::Manager* getManager() const { return mngr_; }

	enum State
	{
		RUNNING,
		PAUSED,
		NEWGAME,
		NEWROUND,
		GAMEOVER
	};

	void setState(State s)
	{
		GameState* new_state = nullptr;
		switch (s)
		{
		case RUNNING:
			new_state = running_state_;
			break;
		case PAUSED:
			new_state = paused_state_;
			break;
		case NEWGAME:
			new_state = newgame_state_;
			break;
		case NEWROUND:
			new_state = newround_state_;
			break;
		case GAMEOVER:
			new_state = gameover_state_;
			break;
		default:
			break;
		}
		if (current_state_ != nullptr)
			current_state_->leave();
		current_state_ = new_state;
		current_state_->enter();
	}

private:
	GameState* current_state_;
	GameState* paused_state_;
	GameState* running_state_;
	GameState* newgame_state_;
	GameState* newround_state_;
	GameState* gameover_state_;
	Game();
	ecs::Manager* mngr_;

	// fachadas
	FighterUtils* f_utils_;
	AsteroidsUtils* a_utils_;
	BlackHoleUtils* bh_utils_;
	MissileUtils* m_utils_;

	friend Singleton;
};

inline Game& g()
{
	return *Game::instance();
}
