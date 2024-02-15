#pragma once
#include <array>
#include "Component.h"
#include "../utils/Vector2D.h"

class Gun : public Component
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
	void reset() const;

	iterator begin()
	{
		return bullets_.begin();
	}

	iterator end()
	{
		return bullets_.end();
	}
	//…
	
private:
	void shoot(Vector2D p, Vector2D v, int width, int height, float r);
	bullets_array bullets_;
	//…
};
