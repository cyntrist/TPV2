#include "Gun.h"

#include <SDL_rect.h>

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/macros.h"
#include "../sdlutils/Texture.h"

Gun::Gun(Texture* img)
	: image_(img)
{
	
}

void Gun::render()
{
	for (auto i : bullets_)
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
	for (auto i : bullets_)
	{
		if (i.used)
		{
			i.pos = i.pos + i.vel;
		}
	}

	if (ih().isKeyDown(SDL_SCANCODE_S))
	{
		//shoot();
	}
}

void Gun::reset() const
{
	for (auto a : bullets_)
		if (a.used) 
			a.used = false;
}

void Gun::shoot(Vector2D p, Vector2D v, int width, int height, float r)
{


}
