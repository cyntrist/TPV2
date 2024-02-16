// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/Manager.h"

namespace ecs
{
	class Manager;
	struct Entity;
}

// when you are not using the methods of a class, just
// say that it exists, that saves time when parsing files
class Container;

class Game {
public:
	Game();
	virtual ~Game();
	void init();
	void start();
private:
	//void checkCollisions();

	ecs::Manager* mngr_;
	ecs::Entity* fighter_;
};

