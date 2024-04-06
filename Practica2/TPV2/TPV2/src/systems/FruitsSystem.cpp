// This file is part of the course TPV2@UCM - Samir Genaim

#include "FruitsSystem.h"
#include "../components/Transform.h"
#include "../ecs/Manager.h"
#include "../sdlutils/SDLUtils.h"
#include "../components/p1/ImageWithFrames.h"
#include "../components/Miraculous.h"

struct Image;
constexpr int FRUIT_SIZE = 20;
constexpr Uint8 NORMAL_FRAME = 12, MIRACULOUS_FRAME = 15;

FruitsSystem::FruitsSystem() :
	gridSide(6), currNumOfFruits_(0)
{
}

FruitsSystem::~FruitsSystem()
{
}

void FruitsSystem::initSystem()
{
}

void FruitsSystem::update()
{
	const auto currTime = sdlutils().currRealTime();

	for (auto e : mngr_->getEntities(ecs::grp::FRUITS))
	{
		auto mc = mngr_->getComponent<Miraculous>(e);
		auto iwf = mngr_->getComponent<ImageWithFrames>(e);
		if (mc != nullptr)
		{
			if (mc->isMiraculous_)
			{ // si esta milagrosa y ha dejado de serlo
				if (mc->timer_ + mc->miraculousDuration_ < currTime)
				{
					mc->resetMiraculous();
					changeFruitSprite(iwf, NORMAL_FRAME);
				}
			}
			else
			{ // si no esta milagrosa y ha de serlo
				if (mc->timer_ + mc->nonMiraculousDuration_ < currTime)
				{
					mc->setMiraculous();
					changeFruitSprite(iwf, MIRACULOUS_FRAME);
				}
			}
		}
	}
}

void FruitsSystem::addFruitGrid(unsigned int n)
{
	gridSide = n;
	const double paddingX = sdlutils().width() / gridSide;
	const double paddingY = sdlutils().height() / gridSide;
	const float iniOffsetX = (paddingX - FRUIT_SIZE) / 2;
	const float iniOffsetY = (paddingY - FRUIT_SIZE) / 2;

	currNumOfFruits_ = 0;
	for (int i = 0; i < gridSide * gridSide; i++)
	{
		auto e = mngr_->addEntity(ecs::grp::FRUITS);
		auto tf = mngr_->addComponent<Transform>(e);
		auto img = mngr_->addComponent<ImageWithFrames>(e, &sdlutils().images().at("atlas"),
		                                                8, 8, 12, 12);
		const int chance = sdlutils().rand().nextInt(0, 10);
		if (chance == 0) // es milagrosa? 1 de cada 10
			mngr_->addComponent<Miraculous>(e);

		tf->init(Vector2D(
			         iniOffsetX + paddingX * (i % gridSide),
			         iniOffsetY + paddingY * (i / gridSide)),
		         Vector2D(),
		         FRUIT_SIZE,
		         FRUIT_SIZE,
		         0.0f
		);
		currNumOfFruits_++;
	}
}

void FruitsSystem::onFruitEaten(ecs::entity_t e)
{
	mngr_->setAlive(e, false);
	currNumOfFruits_--;

	// play sound on channel 1 (if there is something playing there
	// it will be cancelled
	sdlutils().soundEffects().at("eat").play(0, 1);
}

void FruitsSystem::changeFruitSprite(ImageWithFrames* iwf, int frame)
{
	iwf->firstFrame = frame;
	iwf->currentFrame = iwf->firstFrame;
	iwf->lastFrame = iwf->firstFrame;
}

void FruitsSystem::recieve(const Message& m)
{
	switch (m.id)
	{
	case _m_FRUIT_EATEN:
		onFruitEaten(m.fruit_eaten_data.e);
		break;
	case _m_CREATE_FRUIT:
		addFruitGrid(m.create_fruits_data.n);
		break;
	default:
		break;
	}
}
