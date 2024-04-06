// This file is part of the course TPV2@UCM - Samir Genaim

#include "PacManSystem.h"

#include "../components/Image.h"
#include "../components/Transform.h"
#include "../components/p1/HealthComponent.h"
#include "../components/p1/ImageWithFrames.h"
#include "../ecs/Manager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"

constexpr float PACMAN_SPEED = 3.0f;
constexpr int ROT_FACTOR = 90;

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
	resetPosition();
}

void PacManSystem::update()
{
	auto& ihldr = ih();

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

	//if ((pmTR_->vel_.getX() != 0 || pmTR_->vel_.getY() != 0 )&&
	/*lastSound+soundRate < sdlutils().virtualTimer().currTime()) {
	lastSound = sdlutils().virtualTimer().currTime();
	sdlutils().soundEffects().at("chomp").play();*/

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


	//if (ihldr.isKeyDown(SDL_SCANCODE_RIGHT)) { // rotate right
	//	pmTR_->rot_ = pmTR_->rot_ + 5.0f;

	//	// also rotate the PacMan so it looks in the same
	//	// direction where it moves
	//	//
	//	pmTR_->vel_ = pmTR_->vel_.rotate(5.0f);
	//} else if (ihldr.isKeyDown(SDL_SCANCODE_LEFT)) { // rotate left
	//	pmTR_->rot_ = pmTR_->rot_ - 5.0f;

	//	// also rotate the PacMan so it looks in the same
	//	// direction where it moves
	//	//
	//	pmTR_->vel_ = pmTR_->vel_.rotate(-5.0f);
	//} else if (ihldr.isKeyDown(SDL_SCANCODE_UP)) { // increase speed

	//	// add 1.0f to the speed (respecting the limit 3.0f). Recall
	//	// that speed is the length of the velocity vector
	//	float speed = std::min(3.0f, pmTR_->vel_.magnitude() + 1.0f);

	//	// change the length of velocity vecto to 'speed'. We need
	//	// '.rotate(rot)' for the case in which the current speed is
	//	// 0, so we rotate it to the same direction where the PacMan
	//	// is looking
	//	//
	//	pmTR_->vel_ = Vector2D(0, -speed).rotate(pmTR_->rot_);
	//} else if (ihldr.isKeyDown(SDL_SCANCODE_DOWN)) { // decrease speed
	//	// subtract 1.0f to the speed (respecting the limit 0.0f). Recall
	//	// that speed is the length of the velocity vector
	//	float speed = std::max(0.0f, pmTR_->vel_.magnitude() - 1.0f);

	//	// change the length of velocity vector to 'speed'. We need
	//	// '.rotate(rot)' for the case in which the current speed is
	//	// 0, so we rotate it to the same direction where the PacMan
	//	// is looking
	//	//
	//	pmTR_->vel_ = Vector2D(0, -speed).rotate(pmTR_->rot_);
	//}
}

void PacManSystem::recieve(const Message& message)
{
	System::recieve(message);
	if (pacman == nullptr) return;

	switch (message.id)
	{
	case _m_NEW_GAME:
		resetPosition();
		break;
	case _m_ROUND_START:
		resetLives();
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
