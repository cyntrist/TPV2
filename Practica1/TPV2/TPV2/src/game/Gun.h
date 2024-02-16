﻿#pragma once
#include <array>
#include "../ecs/Component.h"
#include "../utils/Vector2D.h"

class Texture;

class Gun : public ecs::Component
{
	//...
public:
	struct Bullet
	{
		bool used = false;
		Vector2D pos;
		Vector2D vel;
		int width;
		int height;
		float rot;
	};

	constexpr static auto max_bullets = 20;
	using bullets_array = std::array<Bullet, max_bullets>;
	using iterator = bullets_array::iterator;

	void render() override;
	void update() override;
	void reset() const;

	iterator begin()
	{
		return bullets_.begin();
	}

	iterator end()
	{
		return bullets_.end();
	}

	Gun(Texture *img);
	~Gun() override = default;
	
private:
	void shoot(Vector2D p, Vector2D v, int width, int height, float r);
	bullets_array bullets_;
	Texture* image_;
	//…
};
