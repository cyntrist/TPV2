#include "ShowAtOppositeSide.h"

#include "../sdlutils/SDLUtils.h"

void ShowAtOppositeSide::initComponent()
{
	transform_ = mngr_->getComponent<Transform>(ent_);
	assert(transform_ != nullptr);
}

void ShowAtOppositeSide::update()
{
	auto &pos = transform_->getPos();
	// se sale por la izq
	if(pos.getX() < 0 - transform_->getHeight())
	{
		pos = Vector2D(sdlutils().width(), pos.getY());
	} // se sasle por la derecha
	else if (pos.getX() > sdlutils().width())
	{
		pos = Vector2D(0, pos.getY());
	}

	// se sale por arriba
	if(pos.getY() < 0 - transform_->getHeight())
	{
		pos = Vector2D(pos.getX(), sdlutils().height());
	} // se sale por abajo
	else if (pos.getY() > sdlutils().height())
	{
		pos = Vector2D(pos.getX(),0);
	}
}
