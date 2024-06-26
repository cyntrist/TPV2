// This file is part of the course TPV2@UCM - Samir Genaim

#include "LittleWolf.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include "Networking.h"
#include "Game.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/Texture.h"

LittleWolf::LittleWolf(uint16_t xres, uint16_t yres, SDL_Window* window,
                       SDL_Renderer* render) :
	walling_width(xres / walling_size_factor), walling_height(
		yres / walling_size_factor), shoot_distace(
		std::min(walling_width, walling_height) / 2), // the shoot distance -- not that it's wrt to the walling size
	map_(), //
	players_(), //
	player_id_(0)
{
	// we start with player 0

	// for some reason it is created with a rotation of 90 degrees -- must be easier to
	// manipulate coordinates
	SDL_Texture* const texture = SDL_CreateTexture(sdlutils().renderer(),
	                                               SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, yres, xres);

	gpu_ = {window, render, texture, xres, yres};

	max_player_life = sdlutils().config().at("max_life");
}

LittleWolf::~LittleWolf()
{
	// nothing to delete, the walling are delete in the Map's destructor
}

void LittleWolf::update()
{
	Player& p = players_[player_id_];
	auto current = sdlutils().virtualTimer().currTime();
	auto diff = restartTimer_ + sdlutils().config().at("restart_time") - current;

	if (restarting_)
	{
		if (Game::instance()->get_networking()->is_master() &&
			diff <= 0.0)
		{
			restarting_ = false;
			send_restart();
		}
	}

	// dead player don't move/spin/shoot
	if (p.state != ALIVE)
		return;


	if (cenitalTimer_ + sdlutils().config().at("cenital_time") <= sdlutils().virtualTimer().currTime())
		cenital_ = false;


	spin(p); // handle spinning
	if (!restarting_)
		move(p); // handle moving
	//shoot(p); // handle shooting
	handle_shoot();
}

void LittleWolf::load(std::string filename)
{
	std::ifstream in(filename);
	if (in.fail())
	{
		throw "Something went wrong while trying to open: " + filename;
	}

	// ** read the user walling

	// read height (rows) and width (cols) of the user walling
	uint16_t uh, uw;
	in >> uh >> uw;

	// remove the new line in the rows/cols line
	in.ignore();

	// buffer to read lines -- we need it because of the extra char
	// for new line that we don't copy to the walling, and this is why its
	// size is uw+1
	auto buffer = new char[uw + 1];

	// rows of the user walling
	auto walling = new uint8_t*[uh];
	for (auto i = 0u; i < uh; i++)
	{
		in.getline(buffer, uw + 1);
		if (buffer[uw] != 0)
		{
			// something is wrong ...
			// in case of error we clean up ...
			delete[] buffer;
			for (auto j = 0u; j < i; j++)
				delete[] walling[i];
			delete[] walling;
			// .. and exit
			throw "There is something wrong with the input file, probably the length does not match what is specified";
		}

		// create and initialize the row
		walling[i] = new uint8_t[uw];
		for (auto j = 0u; j < uw; j++)
			walling[i][j] = buffer[j] - '0';
	}

	// we keep the user walling in the map, just in case it is useful
	// for something -- e.g., draw an upper view skeleton of the map
	map_.user_walling = walling;
	map_.user_walling_width = uw;
	map_.user_walling_height = uh;

	// deallocate buffer, it is not used anymore
	delete[] buffer;

#ifdef _DEBUG
	std::cout << "Loaded the following user walling (" << uh << "x" << uw << ")"
		<< std::endl;
	for (auto i = 0u; i < uh; i++)
	{
		for (auto j = 0u; j < uw; j++)
		{
			std::cout << static_cast<int>(walling[i][j]);
		}
		std::cout << std::endl;
	}
#endif

	// ** construct the walling

	// We fill it all with red tile first, because the walling_height/walling_width might
	// be not divisible by user_walling_height and map_.user_walling_width.
	map_.walling = new uint8_t*[walling_height];
	map_.walling_height = walling_height;
	map_.walling_width = walling_width;
	for (auto i = 0u; i < walling_height; i++)
	{
		map_.walling[i] = new uint8_t[walling_width];
		for (auto j = 0u; j < walling_width; j++)
			map_.walling[i][j] = 1;
	}

	// each tile in the user's walling will be mapped to a block of tiles (cell) of the same color,
	// with the following width and height
	uint16_t cell_height = walling_height / map_.user_walling_height;
	uint16_t cell_width = walling_width / map_.user_walling_width;

	// fill in the walling
	for (int i = 0; i < map_.user_walling_height; i++)
		for (int j = 0; j < map_.user_walling_width; j++)
			for (int k = 0; k < cell_height; k++) // tile (i,j) in the user's walling is mapped to a block of tiles
				for (int l = 0; l < cell_width; l++)
				{
					map_.walling[i * cell_height + k][j * cell_width + l] =
						map_.user_walling[i][j];
				}
}

bool LittleWolf::addPlayer(std::uint8_t id)
{
	assert(id < max_player);

	if (players_[id].state != NOT_USED)
		return false;

	auto& rand = sdlutils().rand();

	// The search for an empty cell start at a random position (orow,ocol)
	uint16_t orow = rand.nextInt(0, map_.walling_height);
	uint16_t ocol = rand.nextInt(0, map_.walling_width);

	// search for an empty cell
	uint16_t row = orow;
	uint16_t col = (ocol + 1) % map_.walling_width;
	while (!((orow == row) && (ocol == col)) && map_.walling[row][col] != 0)
	{
		col = (col + 1) % map_.user_walling_width;
		if (col == 0)
			row = (row + 1) % map_.walling_height;
	}

	// handle the case where the search is failed, which in principle should never
	// happen unless we start with map with few empty cells
	if (row >= map_.walling_height)
		return false;

	// initialize the player
	Player p = {
		//
		id, //
		viewport(0.8f), // focal
		{col + 0.5f, row + 0.5f}, // Where.
		{0.0f, 0.0f}, // Velocity.
		2.0f, // Speed.
		0.9f, // Acceleration.
		0.0f, // Rotation angle in radians.
		ALIVE, // Player state,
		max_player_life
	};

	// not that player <id> is stored in the map as player_to_tile(id) -- which is id+10
	map_.walling[static_cast<int>(p.where.y)][static_cast<int>(p.where.x)] = player_to_tile(id);
	players_[id] = p;

	player_id_ = id;

	if (Game::instance()->get_networking()->is_master())
		std::cout << "**********MASTER**************" << std::endl;
	return true;
}

void LittleWolf::render()
{
	// if the player is dead we only render upper view, otherwise the normal view
	if (players_[player_id_].state == DEAD || (cenital_
		&& cenitalTimer_ + sdlutils().config().at("cenital_time") > sdlutils().virtualTimer().currTime()))
		render_upper_view();
	else
		render_map(players_[player_id_]);

	// render the identifiers, state, etc
	render_players_info();

	if (restarting_)
		render_warning();
}

LittleWolf::Hit LittleWolf::cast(const Point where, Point direction,
                                 uint8_t** walling, bool ignore_players, bool ignore_deads)
{
	// Determine whether to step horizontally or vertically on the grid.
	Point hor = sh(where, direction);
	Point ver = sv(where, direction);
	Point ray = mag(sub(hor, where)) < mag(sub(ver, where)) ? hor : ver;
	// Due to floating point error, the step may not make it to the next grid square.
	// Three directions (dy, dx, dc) of a tiny step will be added to the ray
	// depending on if the ray hit a horizontal wall, a vertical wall, or the corner
	// of two walls, respectively.
	Point dc = mul(direction, 0.01f);
	Point dx = {dc.x, 0.0f};
	Point dy = {0.0f, dc.y};
	Point test = add(ray,
	                 // Tiny step for corner of two grid squares.
	                 mag(sub(hor, ver)) < 1e-3f
		                 ? dc
		                 :
		                 // Tiny step for vertical grid square.
		                 dec(ray.x) == 0.0f
		                 ? dx
		                 :
		                 // Tiny step for a horizontal grid square.
		                 dy);
	const Hit hit = {tile(test, walling), ray};
	// If a wall was not hit, then continue advancing the ray.

	if (hit.tile > 0 && hit.tile < 10)
	{
		return hit;
	}
	if (hit.tile > 9 && !ignore_players
		&& (!ignore_deads || players_[hit.tile - 10].state != DEAD))
	{
		return hit;
	}
	return cast(ray, direction, walling, ignore_players, ignore_deads);
}

void LittleWolf::send_my_info()
{
	Player& p = players_[player_id_];

	Game::instance()->get_networking()->send_my_info(
		Vector2D(
			p.where.x,
			p.where.y),
		Vector2D(
			p.velocity.x,
			p.velocity.y),
		p.speed,
		p.acceleration,
		p.theta,
		p.state,
		p.life);
}

void LittleWolf::send_sync() const
{
	for (uint8_t i = 0; i < max_player; i++)
		if (players_[i].state != NOT_USED)
			Game::instance()->get_networking()->send_sync(
				i,
				Vector2D(
					players_[i].where.x,
					players_[i].where.y
				)
			);
}

void LittleWolf::update_player_info(uint8_t playerID, float posX, float posY, float velX, float velY, float speed,
                                float acceleration, float theta, PlayerState state, int life)
{
	if (players_[playerID].state == NOT_USED)
	{
		// initialize the player
		Player p = {
			//
			playerID, //
			viewport(0.8f), // focal
			{posX + 0.5f, posY + 0.5f}, // Where.
			{velX, velY}, // Velocity.
			speed, // Speed.
			acceleration, // Acceleration.
			theta, // Rotation angle in radians.
			ALIVE, // Player state
			life
		};

		// not that player <id> is stored in the map as player_to_tile(id) -- which is id+10
		map_.walling[static_cast<int>(p.where.y)][static_cast<int>(p.where.x)] = player_to_tile(playerID);
		players_[playerID] = p;
	}
	else
	{
		auto& p = players_[playerID];

		if (Game::instance()->get_networking()->is_master())
		{
			const Point current = p.where;
			constexpr Point none = {0.0f, 0.0f};

			if (tile(p.where, map_.walling) != player_to_tile(playerID)
				|| tile(p.where, map_.walling) != 0) {
				p.velocity = none;
				p.where = current;
				std::cout << "COLISION COLISION COLISION" << std::endl;
				send_sync();
				return;
			}	
		}

		map_.walling[static_cast<int>(p.where.y)][static_cast<int>(p.where.x)] = 0;

		p.where.x = posX;
		p.where.y = posY;

		p.velocity.x = velX;
		p.velocity.y = velY;

		p.speed = speed;
		p.acceleration = acceleration;
		p.theta = theta;
		p.state = state;
		p.life = life;

		map_.walling[static_cast<int>(p.where.y)][static_cast<int>(p.where.x)] = player_to_tile(playerID);
	}
}

void LittleWolf::disconnect_player(uint8_t id)
{
	auto& p = players_[id];
	auto x = static_cast<int>(p.where.x);
	auto y = static_cast<int>(p.where.y);
	map_.walling[x][y] = 0;
	p.state = NOT_USED;
}

void LittleWolf::update_sync(uint8_t id, const Vector2D& pos)
{
	map_.walling[static_cast<int>(players_[id].where.y)][static_cast<int>(players_[id].where.x)] = 0;
	players_[id].where.x = pos.getX();
	players_[id].where.y = pos.getY();
	map_.walling[static_cast<int>(players_[id].where.y)][static_cast<int>(players_[id].where.x)] = player_to_tile(id);
}

void LittleWolf::send_shoot_request()
{
	Game::instance()->get_networking()->send_shoot_request();
}

void LittleWolf::handle_shoot_request(uint8_t id)
{
	shoot(players_[id]);
}

void LittleWolf::send_player_death(uint8_t id)
{
	Game::instance()->get_networking()->send_player_death(id);
}

void LittleWolf::send_upcoming_restart()
{
	Game::instance()->get_networking()->send_upcoming_restart();
}

void LittleWolf::send_restart()
{
	restarting_ = false;

	for (int i = 0; i < max_player; i++)
	{
		//assert(i < max_player);

		if (players_[i].state == NOT_USED || players_[i].state == DEAD)
			continue;

		auto& p = players_[i];
		auto& rand = sdlutils().rand();

		// The search for an empty cell start at a random position (orow,ocol)
		uint16_t orow = rand.nextInt(0, map_.walling_height);
		uint16_t ocol = rand.nextInt(0, map_.walling_width);

		// search for an empty cell
		uint16_t row = orow;
		uint16_t col = (ocol + 1) % map_.walling_width;
		while (!(orow == row && ocol == col) && map_.walling[row][col] != 0)
		{
			col = (col + 1) % map_.user_walling_width;
			if (col == 0)
				row = (row + 1) % map_.walling_height;
		}

		//		// handle the case where the search is failed, which in principle should never
		//// happen unless we start with map with few empty cells
		//if (row >= map_.walling_height)
		//	return false;

		//// initialize the player
		//Player p = {
		//	//
		//	id, //
		//	viewport(0.8f), // focal
		//	{col + 0.5f, row + 0.5f}, // Where.
		//	{0.0f, 0.0f}, // Velocity.
		//	2.0f, // Speed.
		//	0.9f, // Acceleration.
		//	0.0f, // Rotation angle in radians.
		//	ALIVE // Player state
		//};

		//// not that player <id> is stored in the map as player_to_tile(id) -- which is id+10
		//map_.walling[static_cast<int>(p.where.y)][static_cast<int>(p.where.x)] = player_to_tile(id);
		//players_[id] = p;

		//player_id_ = id;

		//return true;

		map_.walling[static_cast<int>(p.where.y)][static_cast<int>(p.where.x)] = 0;

		p.where.x = col + 0.5f;
		p.where.y = row + 0.5f;

		p.velocity.x = 0;
		p.velocity.y = 0;
		p.speed = 2.0;
		p.acceleration = 0.9;
		p.theta = 0;
		p.state = ALIVE;
		p.life = max_player_life;

		// not that player <id> is stored in the map as player_to_tile(id) -- which is id+10
		map_.walling[static_cast<int>(p.where.y)][static_cast<int>(p.where.x)] = player_to_tile(i);

		Game::instance()->get_networking()->send_sync(i, Vector2D(p.where.x, p.where.y));
	}
	Game::instance()->get_networking()->send_restart();
}

void LittleWolf::handle_player_death(uint8_t id)
{
	players_[id].state = DEAD;
}

void LittleWolf::handle_shoot()
{
	if (ih().keyDownEvent() && ih().isKeyDown(SDL_SCANCODE_SPACE))
		send_shoot_request();
}

void LittleWolf::handle_upcoming_restart()
{
	std::cout << "HANDLING UPCOMING RESTART" << std::endl;
	restarting_ = true;
	restartTimer_ = sdlutils().virtualTimer().currTime();
	//sdlutils().config().at("restart_time");
}

void LittleWolf::handle_restart()
{
	restarting_ = false;
	std::cout << "HANDLING RESTART" << std::endl;
	for (auto& p : players_)
		if (p.state != NOT_USED && p.state != DEAD)
		{
			p.state = ALIVE;
			p.life = max_player_life;
		}
		else if (p.state == DEAD)
		{
			p.where.x = 0;
			p.where.y = 0;
		}
}

LittleWolf::Wall LittleWolf::project(const int xres, const int yres,
                                     const float focal, const Point corrected)
{
	// Normal distance of corrected ray is clamped to some small value else wall size will shoot to infinity.
	const float normal = corrected.x < 1e-2f ? 1e-2f : 0.05 * corrected.x;
	const float size = 0.5f * focal * xres / normal;
	const int top = (yres + size) / 2.0f;
	const int bot = (yres - size) / 2.0f;
	// Top and bottom values are clamped to screen size else renderer will waste cycles
	// (or segfault) when rasterizing pixels off screen.
	const Wall wall = {top > yres ? yres : top, bot < 0 ? 0 : bot, size};
	return wall;
}

void LittleWolf::render_map(Player& p)
{
	// lock the texture
	const Display display = lock(gpu_);

	const Line camera = rotate(p.fov, p.theta);

	// Ray cast for all columns of the window.
	for (int x = 0; x < gpu_.xres; x++)
	{
		//		for (int y = 0; y < gpu_.yres; y++)
		//			put(display, x, y, 0x00000000);

		// draw walls
		const Point direction = lerp(camera, x / static_cast<float>(gpu_.xres));
		const Hit hit = cast(p.where, direction, map_.walling, true, false);
		const Point ray = sub(hit.where, p.where);
		const Point corrected = turn(ray, -p.theta);
		const Wall wall = project(gpu_.xres, gpu_.yres, p.fov.a.x, corrected);

		for (int y = 0; y < wall.bot; y++)
			put(display, x, y, 0x00000000);

		for (int y = wall.bot; y < wall.top; y++)
			put(display, x, y, color(hit.tile));

		for (int y = wall.top; y < gpu_.yres; y++)
			put(display, x, y, 0x00000000);

		// draw players
		const Point direction_p = lerp(camera, x / static_cast<float>(gpu_.xres));
		const Hit hit_p = cast(p.where, direction_p, map_.walling, false,
		                       false);
		const Point ray_p = sub(hit_p.where, p.where);
		const Point corrected1 = turn(ray_p, -p.theta);
		const Wall wall_p = project(gpu_.xres, gpu_.yres, p.fov.a.x,
		                            corrected1);

		if (hit_p.tile >= 10)
		{
			int size_p = 0;
			uint8_t id = hit_p.tile - 10;

			if (players_[id].state == ALIVE)
				size_p = wall_p.top - (wall_p.top - wall_p.bot) / 3; //
			else
				size_p = wall_p.bot + (wall_p.top - wall_p.bot) / 10; // when it is dead it is shorter

			for (int y = wall_p.bot; y < size_p; y++)
				put(display, x, y, color(hit_p.tile));
		}
	}

	// draw a rifle sight at the center
	for (int i = -10; i < 10; i++)
	{
		put(display, gpu_.xres / 2, gpu_.yres / 2 + i, 0xAAAAAAAA);
		put(display, gpu_.xres / 2 + i, gpu_.yres / 2, 0xAAAAAAAA);
	}

	// unlock the texture
	unlock(gpu_);

	// copy the texture to the renderer
	const SDL_Rect dst = {
		(gpu_.xres - gpu_.yres) / 2, (gpu_.yres - gpu_.xres)
		/ 2,
		gpu_.yres, gpu_.xres,
	};
	SDL_RenderCopyEx(gpu_.renderer, gpu_.texture, nullptr, &dst, -90, nullptr,
	                 SDL_FLIP_NONE);
}

void LittleWolf::render_upper_view()
{
	// lock texture
	const Display display = lock(gpu_);

	for (auto x = 0u; x < gpu_.xres; x++)
		for (auto y = 0u; y < gpu_.yres; y++)
			put(display, x, y, 0x00000000);


	for (auto x = 0u; x < map_.walling_height; x++)
		for (auto y = 0u; y < map_.walling_width; y++)
		{
			// each non empty position in the walling is drawn as a square in the window,
			// because the walling size is smaller than the resolution by 'walling_size_factor'
			if (map_.walling[x][y] != 0)
				for (int i = 0; i < walling_size_factor; i++)
					for (int j = 0; j < walling_size_factor; j++)
						put(display, y * walling_size_factor + i,
						    gpu_.yres - 1 - x * walling_size_factor + j,
						    color(map_.walling[x][y]));
		}

	// unlock texture
	unlock(gpu_);

	const SDL_Rect dst = {
		(gpu_.xres - gpu_.yres) / 2, (gpu_.yres - gpu_.xres)
		/ 2,
		gpu_.yres, gpu_.xres,
	};
	SDL_RenderCopyEx(gpu_.renderer, gpu_.texture, nullptr, &dst, -90, nullptr,
	                 SDL_FLIP_NONE);

	// add labels to each player, with corresponding rotation
	for (int i = 0u; i < max_player; i++)
	{
		Player& p = players_[i];
		if (p.state != NOT_USED)
		{
			Texture info(sdlutils().renderer(), "P" + std::to_string(i),
			             sdlutils().fonts().at("ARIAL12"),
			             build_sdlcolor(color_rgba(i + 10)));

			int w = info.width();
			int h = info.height();

			SDL_Rect src = build_sdlrect(0.0f, 0.0f, w, h);
			SDL_Rect dest = build_sdlrect(p.where.x * 2 - w / 2,
			                              p.where.y * 2 - h / 2, w, h);

			info.render(src, dest,
			            p.theta * 180 / 3.14159265358979323846264338327950288f);
		}
	}
}

void LittleWolf::render_players_info()
{
	uint_fast16_t y = 0;

	for (auto i = 0u; i < max_player; i++)
	{
		PlayerState s = players_[i].state;

		// render player info if it is used
		if (s != NOT_USED)
		{
			std::string msg = (i == player_id_ ? "*P" : " P")
				+ std::to_string(i) + (s == DEAD
					                       ? " (dead)"
					                       : " / LIFE: " + std::to_string(players_[i].life)
					                       + "");

			Texture info(sdlutils().renderer(), msg,
			             sdlutils().fonts().at("ARIAL24"),
			             build_sdlcolor(color_rgba(i + 10)));

			SDL_Rect dest = build_sdlrect(0, y, info.width(), info.height());

			info.render(dest);
			y += info.height() + 5;
		}
	}
}

void LittleWolf::render_warning()
{
	auto current = sdlutils().virtualTimer().currTime();
	auto diff = restartTimer_ + sdlutils().config().at("restart_time") - current;
	auto remaining = std::to_string(diff / 1000);
	std::string text = "RESTARTING IN " + remaining + " SECONDS.";

	Texture info(
		sdlutils().renderer(),
		text,
		sdlutils().fonts().at("ARIAL48"),
		build_sdlcolor(color_rgba(0 + 10)));

	SDL_Rect dest = build_sdlrect(
		(sdlutils().width() - info.width()) / 2,
		(sdlutils().height() - info.height()) / 2,
		info.width(),
		info.height()
	);

	info.render(dest);
}

void LittleWolf::move(Player& p)
{
	auto& ihdrl = ih();

	// W forwards, S backwards, D right, L left

	const Point last = p.where, zero = {0.0f, 0.0f};

	// Accelerates with key held down.
	if (ihdrl.isKeyDown(SDL_SCANCODE_W) || ihdrl.isKeyDown(SDL_SCANCODE_S)
		|| ihdrl.isKeyDown(SDL_SCANCODE_D)
		|| ihdrl.isKeyDown(SDL_SCANCODE_A))
	{
		constexpr Point reference = {1.0f, 0.0f};
		const Point direction = turn(reference, p.theta);
		const Point acceleration = mul(direction, p.acceleration);
		if (ihdrl.isKeyDown(SDL_SCANCODE_W))
			p.velocity = add(p.velocity, acceleration);
		if (ihdrl.isKeyDown(SDL_SCANCODE_S))
			p.velocity = sub(p.velocity, acceleration);
		if (ihdrl.isKeyDown(SDL_SCANCODE_D))
			p.velocity = add(p.velocity, rag(acceleration));
		if (ihdrl.isKeyDown(SDL_SCANCODE_A))
			p.velocity = sub(p.velocity, rag(acceleration));
	}
	else
	{
		// Otherwise, decelerates (exponential decay).
		p.velocity = mul(p.velocity, 1.0f - p.acceleration / p.speed);
	}

	// Caps velocity if top speed is exceeded.
	if (mag(p.velocity) > p.speed)
		p.velocity = mul(unit(p.velocity), p.speed);

	// Moves.
	p.where = add(p.where, p.velocity);
	// Sets velocity to zero if there is a collision and puts p back in bounds.

	// if player hits a wall or a different player, we take the player back
	// to previous position and put velocity to 0
	if (tile(p.where, map_.walling) != 10 + player_id_
		&& tile(p.where, map_.walling) != 0)
	{
		p.velocity = zero;
		p.where = last;
	}
	else
	{
		// otherwise we make a move
		int y0 = static_cast<int>(last.y);
		int x0 = static_cast<int>(last.x);
		int y1 = static_cast<int>(p.where.y);
		int x1 = static_cast<int>(p.where.x);
		if (x0 != x1 || y0 != y1)
		{
			map_.walling[y1][x1] = map_.walling[y0][x0];
			map_.walling[y0][x0] = 0;
		}
	}
	send_my_info();
}

void LittleWolf::spin(Player& p)
{
	auto& ihdrl = ih();

	// L spin right, H spin left -- when left shift is held the player spins slowly

	// turn by 0.05rad, but if left shift is pressed make is 0.005rad
	float d = 0.05f;
	if (ihdrl.isKeyDown(SDL_SCANCODE_LSHIFT))
	{
		d = 0.005f;
	}

	if (ihdrl.isKeyDown(SDL_SCANCODE_J))
		p.theta -= d;
	if (ihdrl.isKeyDown(SDL_SCANCODE_K))
		p.theta += d;
}

bool LittleWolf::shoot(Player& p)
{
	// play gun shot sound
	sdlutils().soundEffects().at("gunshot").play();

	// we shoot in several directions, because with projection what you see is not exact
	for (float d = -0.05; d <= 0.05; d += 0.005)
	{
		// search which tile was hit
		const Line camera = rotate(p.fov, p.theta + d);
		Point direction = lerp(camera, 0.5f);
		direction.x = direction.x / mag(direction);
		direction.y = direction.y / mag(direction);
		const Hit hit = cast(p.where, direction, map_.walling, false, true);
		auto distance = mag(sub(p.where, hit.where));
#if _DEBUG
		printf("Shoot by player %d hit a tile with value %d! at distance %f\n", p.id, hit.tile,
		       mag(sub(p.where, hit.where)));
#endif

		// if we hit a tile with a player id and the distance from that tile is smaller
		// than shoot_distace, we mark the player as dead
		if (hit.tile > 9 && mag(sub(p.where, hit.where)) < shoot_distace)
		{
			uint8_t id = tile_to_player(hit.tile);
			//players_[id].state = DEAD;
			auto volume = shoot_distace - distance / 5;
			sdlutils().soundEffects().at("pain").setVolume(volume);
			sdlutils().soundEffects().at("pain").play();


			int alivePlayers = 0;
			for (auto& p : players_)
				if (p.state == ALIVE)
					alivePlayers++;

			int damage = sdlutils().config().at("hit_damage") - distance / 10;

			players_[id].life -= damage;

			std::cout << players_[id].life << std::endl;
			//if (players_[id].life <= 0)
			{
				send_player_death(id);
				players_[id].state = DEAD;
			}

			int currentPlayers = 0;
			for (auto& p : players_)
				if (p.state == ALIVE)
					currentPlayers++;

			if (currentPlayers == 2 && currentPlayers != alivePlayers)
			{
				send_upcoming_restart();
			}
			else if (currentPlayers <= 1)
			{
				//bringAllToLife();
				//send_upcoming_restart();
			}
			std::cout << players_[id].life << std::endl;
			return true;
		}
	}

	return false;
}

void LittleWolf::bringAllToLife()
{
	// bring all dead players to life -- all stay in the same position
	for (auto& player : players_)
		if (player.state == DEAD)
		{
			player.state = ALIVE;
			player.life = max_player_life;
		}
}
