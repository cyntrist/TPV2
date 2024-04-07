#include "ImmunitySystem.h"

#include "../components/Immune.h"
#include "../components/Miraculous.h"
#include "../components/p1/ImageWithFrames.h"
#include "../ecs/Manager.h"

ImmunitySystem::ImmunitySystem()
{
}

void ImmunitySystem::initSystem()
{
	pacman = mngr_->getHandler(ecs::hdlr::PACMAN);
	assert(pacman != nullptr);
	ic = mngr_->getComponent<Immune>(pacman);
	assert(ic != nullptr);
}

void ImmunitySystem::update()
{
	const auto currTime = sdlutils().currRealTime();
	if (ic != nullptr)
	{
		if (!ic->isImmune_) return;
		if (ic->timer_ + IMMUNITY_DURATION < currTime)
		{ // si esta inmune, contamos a ver si ya se le ha pasado
			ic->resetImmune();
			changePacmanSprite(NORMAL_FIRST_FRAME, NORMAL_LAST_FRAME);
		}
		// si esta inmune pero no ha pasado el tiempo no pasa nada
	}
}

void ImmunitySystem::recieve(const Message& message)
{
	auto e = message.fruit_eaten_data.e;
	if (message.id == _m_FRUIT_EATEN)
	{
		// si el evento es de comer fruta
		auto mc = mngr_->getComponent<Miraculous>(e);
		if (mc != nullptr) // si la fruta es milagrosa
		{
			// ahora el pacman es inmune
			ic->setImmune();
			changePacmanSprite(IMMUNE_FIRST_FRAME, IMMUNE_LAST_FRAME);
		} // si no es milagrosa no pasa nada
	}
}

void ImmunitySystem::changePacmanSprite(int first, int last)
{
	auto iwf = mngr_->getComponent<ImageWithFrames>(pacman);
	iwf->firstFrame = first;
	iwf->lastFrame = last;
	iwf->currentFrame = first;
}
