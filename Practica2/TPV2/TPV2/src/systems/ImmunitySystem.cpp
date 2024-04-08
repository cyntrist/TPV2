#include "ImmunitySystem.h"

#include "../components/Immune.h"
#include "../components/Miraculous.h"
#include "../components/p1/ImageWithFrames.h"
#include "../ecs/Manager.h"

ImmunitySystem::ImmunitySystem() = default;

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
		{
			// si esta inmune, contamos a ver si ya se le ha pasado
			resetImmune();
		} // si esta inmune pero no ha pasado el tiempo no pasa nada
	}
}

void ImmunitySystem::recieve(const Message& message)
{
	auto e = message.entity_collided_data.e;
	if (message.id == _m_FRUIT_EATEN)
	{
		// si el evento es de comer fruta
		auto mc = mngr_->getComponent<Miraculous>(e);
		if (mc != nullptr && !ic->isImmune_) // si la fruta es milagrosa
		{
			// ahora el pacman es inmune
			setImmune();
		} // si no es milagrosa no pasa nada
	}
	else if (message.id == _m_ROUND_START || message.id == _m_NEW_GAME)
		resetImmune();
}

void ImmunitySystem::changePacmanSprite(int first, int last)
{
	auto iwf = mngr_->getComponent<ImageWithFrames>(pacman);
	iwf->firstFrame = first;
	iwf->lastFrame = last;
	iwf->currentFrame = first;
}

void ImmunitySystem::setImmune()
{
	ic->setImmune();
	changePacmanSprite(IMMUNE_FIRST_FRAME, IMMUNE_LAST_FRAME);
	mngr_->send(Message{_m_IMMUNITY_START});
}

void ImmunitySystem::resetImmune()
{
	ic->resetImmune();
	changePacmanSprite(NORMAL_FIRST_FRAME, NORMAL_LAST_FRAME);
	mngr_->send(Message{_m_IMMUNITY_END});
}
