// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <SDL_stdinc.h>
#include "../utils/Singleton.h"

class LittleWolf;
class Networking;

class Game : public Singleton<Game> {
	friend Singleton;
	Game();
public:
	virtual ~Game();
	bool init(const char *host, Uint16 port);
	void start();

	/// getters...
	Networking* get_networking() const { return net_; }
	LittleWolf* get_little_wolf() const { return little_wolf_; }
private:
	LittleWolf *little_wolf_;
	Networking *net_;
};

