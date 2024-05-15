// This file is part of the course TPV2@UCM - Samir Genaim

#include "PacManSystem.h"
#include "../components/Transform.h"
#include "../components/p1/HealthComponent.h"
#include "../components/p1/ImageWithFrames.h"
#include "../ecs/Manager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../components/Immune.h"
constexpr float PACMAN_SPEED = 3.0f;
constexpr int ROT_FACTOR = 90,
			  CHOMP_TIMER = 500;

PacManSystem::PacManSystem() :
	pmTR_(nullptr)
{
}

PacManSystem::~PacManSystem()
{
}

void PacManSystem::initSystem()
{
	// create the PacMan entity
	//
	pacman = mngr_->addEntity();
	mngr_->setHandler(ecs::hdlr::PACMAN, pacman);
	pmTR_ = mngr_->addComponent<Transform>(pacman);

	mngr_->addComponent<ImageWithFrames>(pacman, &sdlutils().images().at("atlas"),
	                                     8, 8, 0, 3);
	mngr_->addComponent<HealthComponent>(pacman, &sdlutils().images().at("heart"),
	                                     3);
	mngr_->addComponent<Immune>(pacman);
	resetPosition();
}

void PacManSystem::update()
{
	auto& ihldr = ih();
	auto currTime = sdlutils().virtualTimer().currTime();

	/// GESTION DE INPUT
	/// ARRIBA
	if (ihldr.isKeyDown(SDL_SCANCODE_UP))
	{
		pmTR_->vel_.set(0, -PACMAN_SPEED);
		pmTR_->vel_ = pmTR_->vel_.rotate(pmTR_->rot_);
	}
	/// DERECHA
	if (ihldr.isKeyDown(SDL_SCANCODE_RIGHT) && !right)
	{
		right = true;
		pmTR_->rot_ += ROT_FACTOR;
		pmTR_->vel_ = pmTR_->vel_.rotate(ROT_FACTOR);
	}
	/// ABAJO
	if (ihldr.isKeyDown(SDL_SCANCODE_DOWN))
		pmTR_->vel_.set(0, 0);
	/// IZQUIERDA
	if (ihldr.isKeyDown(SDL_SCANCODE_LEFT) && !left)
	{
		left = true;
		pmTR_->rot_ -= ROT_FACTOR;
		pmTR_->vel_ = pmTR_->vel_.rotate(-ROT_FACTOR);
	}
	/// NADA
	if (ihldr.isKeyUp(SDL_SCANCODE_RIGHT))
		right = false;
	if (ihldr.isKeyUp(SDL_SCANCODE_LEFT))
		left = false;

	if (!(pmTR_->vel_.getX() == 0.0 
		&& pmTR_->vel_.getY() == 0.0) 
		&&soundTimer + CHOMP_TIMER < currTime) 
	{
		soundTimer = currTime;
		sdlutils().soundEffects().at("chomp").play();
	}

	pmTR_->pos_ = pmTR_->pos_ + pmTR_->vel_; /// ACTUAL PACMAN MOVEMENT!!!

	// check left/right borders
	if (pmTR_->pos_.getX() < 0)
	{
		pmTR_->pos_.setX(0.0f);
		pmTR_->vel_.set(0.0f, 0.0f);
	}
	else if (pmTR_->pos_.getX() + pmTR_->width_ > sdlutils().width())
	{
		pmTR_->pos_.setX(sdlutils().width() - pmTR_->width_);
		pmTR_->vel_.set(0.0f, 0.0f);
	}

	// check upper/lower borders
	if (pmTR_->pos_.getY() < 0)
	{
		pmTR_->pos_.setY(0.0f);
		pmTR_->vel_.set(0.0f, 0.0f);
	}
	else if (pmTR_->pos_.getY() + pmTR_->height_ > sdlutils().height())
	{
		pmTR_->pos_.setY(sdlutils().height() - pmTR_->height_);
		pmTR_->vel_.set(0.0f, 0.0f);
	}
}

void PacManSystem::recieve(const Message& message)
{
	System::recieve(message);
	if (pacman == nullptr) return;

	switch (message.id)
	{
	case _m_NEW_GAME:
		if (mngr_->getComponent<HealthComponent>(pacman)->getLives() <= 0)
			resetLives();
		resetPosition();
		break;
	case _m_ROUND_START:
		resetPosition();
		break;
	default:
		break;
	}
}

void PacManSystem::resetPosition()
{
	auto iwf = mngr_->addComponent<ImageWithFrames>(pacman, &sdlutils().images().at("atlas"),
	                                                8, 8, 0, 3);
	auto w = iwf->frameWidth_ / 2;
	auto h = iwf->frameHeight_ / 2;
	auto x = (sdlutils().width() - w) / 2.0f;
	auto y = (sdlutils().height() - h) / 2.0f;
	pmTR_->init(Vector2D(x, y), Vector2D(),
	            w, h, 0.0f);
}

void PacManSystem::resetLives()
{
	mngr_->getComponent<HealthComponent>(pacman)->resetLives();
}
