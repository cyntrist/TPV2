// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/Manager.h"
#include "../utils/Singleton.h"

namespace ecs
{
	class Manager;
	struct Entity;
}

// when you are not using the methods of a class, just
// say that it exists, that saves time when parsing files
class Container;

class Game
{
public:
	virtual ~Game();
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
		//GameState* new_state = nullptr;
		switch (s)
		{
		case RUNNING:
			//new_state = runing_state_;
			break;
		case PAUSED:
			//new_state = paused_state_;
			break;
		case NEWGAME:
			//new_state = newgame_state_;
			break;
		case NEWROUND:
			//new_state = newround_state_;
			break;
		case GAMEOVER:
			//new_state = gameover_state_;
			break;
		default:
			break;
		}
		/*current_state_->leave();
		current_state_ = new_state;
		current_state_->enter();*/
	}

private:
	//GameState *current_state_;
	//GameState *paused_state_;
	/*GameState *runing_state_;
	GameState *newgame_state_;
	GameState *newround_state_;
	GameState *gameover_state_;*/
	Game();
	void checkCollisions();
	ecs::Manager* mngr_;
	ecs::Entity* fighter_;
};
