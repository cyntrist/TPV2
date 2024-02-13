#include "ShowAtOppositeSide.h"

#include "src/game/Container.h"
#include "src/sdlutils/SDLUtils.h"

void ShowAtOppositeSide::update(Container* o)
{
	auto &pos = o->getPos();
	// se sale por la izq
	if(pos.getX() < 0)
	{
		//pos.getX() = sdlutils().width();
	} // se sasle por la derecha
	else if (pos.getX() > sdlutils().width())
	{
		
	}

	// se sale por arriba
	if(pos.getY() < 0)
	{
		
	} // se sale por abajo
	else if (pos.getY() > sdlutils().height())
	{
		
	}
}
