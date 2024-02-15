#include "Gun.h"

void Gun::reset() const
{
	for (auto a : bullets_)
		if (a.used) 
			a.used = false;
}

void Gun::shoot(Vector2D p, Vector2D v, int width, int height, float r)
{


}
