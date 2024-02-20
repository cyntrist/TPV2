#include "Gun.h"

#include <SDL_rect.h>
#include "../ecs/Manager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/macros.h"
#include "../sdlutils/Texture.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Collisions.h"

constexpr int BULLET_SIZE = 50,
              SHOOT_TIMER = 250;

Gun::Gun(Texture* img)
	: bullets_(),
	  image_(img),
	  transform_(),
	  lastShot_(-1),
	  lastIndex_(0)
{
}

void Gun::initComponent()
{
	transform_ = mngr_->getComponent<Transform>(ent_);
	assert(transform_ != nullptr);
}

void Gun::render()
{
	for (auto& i : bullets_)
	{
		if (i.used)
		{
			SDL_Rect rect = build_sdlrect(i.pos, i.width, i.height);
			image_->render(rect, i.rot);
		}
	}
}

void Gun::update()
{
	if (ih().isKeyDown(SDL_SCANCODE_S))
	{
		Vector2D p = transform_->getPos();
		Vector2D v = transform_->getVel();
		float w = transform_->getWidth();
		float h = transform_->getHeight();
		float r = transform_->getRot();
		int bw = 5;
		int bh = 20;
		Vector2D c = p + Vector2D(w / 2.0f, h / 2.0f);
		Vector2D bp = c - Vector2D(bw / 2, h / 2.0f + 5.0f + bh).rotate(r) - Vector2D(bw / 2, bh / 2);
		Vector2D bv = Vector2D(0, -1).rotate(r) * (v.magnitude() + 5.0f);
		float br = Vector2D(0, -1).angle(bv);
		shoot(bp, bv, bw, bh, br);
	}

	for (Bullet& i : bullets_)
	{
		if (i.used)
		{
			i.pos = i.pos + i.vel;
			if (!Collisions::collides(
				Vector2D(0, 0),
				sdlutils().width(),
				sdlutils().height(),
				i.pos,
				i.width,
				i.height
			))
			{
				i.used = false;
			}
		}
	}
}

void Gun::reset() const
{
	for (auto a : bullets_)
	{
		if (a.used)
			a.used = false;		
	}

}

void Gun::shoot(Vector2D p, Vector2D v, int width, int height, float r)
{
	// ha pasado el cooldown?
	const int newShot = sdlutils().currRealTime();
	if (newShot < lastShot_ + SHOOT_TIMER)
		return;

	// dispara
	lastShot_ = newShot;

	int i = (lastIndex_ + 1) % max_bullets;
	while (i != lastIndex_ && bullets_[i].used)
		i = (i + 1) % max_bullets;

	if (i < max_bullets)
	{
		Bullet& bullet = bullets_[i];
		bullet.used = true;
		bullet.pos = p;
		bullet.vel = v;
		bullet.width = width;
		bullet.height = height;
		bullet.rot = r;

		lastIndex_ = i;
		sdlutils().soundEffects().at("fire").play();
	}
}
