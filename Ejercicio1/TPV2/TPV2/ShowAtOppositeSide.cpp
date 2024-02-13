#include "ShowAtOppositeSide.h"

#include "src/game/Container.h"
#include "src/sdlutils/SDLUtils.h"

void ShowAtOppositeSide::update(Container* o)
{
	auto &pos = o->getPos();
	// se sale por la izq
	if(pos.getX() < 0 - o->getHeight())
	{
		pos = Vector2D(sdlutils().width(), pos.getY());
	} // se sasle por la derecha
	else if (pos.getX() > sdlutils().width())
	{
		pos = Vector2D(0, pos.getY());
	}

	// se sale por arriba
	if(pos.getY() < 0 - o->getHeight())
	{
		pos = Vector2D(pos.getX(), sdlutils().height());
	} // se sale por abajo
	else if (pos.getY() > sdlutils().height())
	{
		pos = Vector2D(pos.getX(),0);
	}
}
