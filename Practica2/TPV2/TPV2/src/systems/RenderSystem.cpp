// This file is part of the course TPV2@UCM - Samir Genaim

#include "RenderSystem.h"
#include "../components/Image.h"
#include "../components/p1/ImageWithFrames.h"
#include "../components/Transform.h"
#include "../ecs/Manager.h"
#include "../sdlutils/macros.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/Texture.h"
#include "GameCtrlSystem.h"
#include "../components/p1/HealthComponent.h"

constexpr Uint32 FRAME_DURATION = 100;

RenderSystem::RenderSystem() {

}

RenderSystem::~RenderSystem() {
}

void RenderSystem::initSystem() {
}

void RenderSystem::update() {
	drawMsgs();
	drawFruits();
	drawPacMan();
	drawGhosts();
}

void RenderSystem::drawFruits() {
	// draw stars
	for (auto e : mngr_->getEntities(ecs::grp::FRUITS)) 
	{
		auto iwf = mngr_->getComponent<ImageWithFrames>(e);
		drawImageWithFrames(iwf);
	}
}

void RenderSystem::drawPacMan() {
	auto e = mngr_->getHandler(ecs::hdlr::PACMAN);
	auto tr = mngr_->getComponent<Transform>(e);
	auto iwf = mngr_->getComponent<ImageWithFrames>(e);
	auto hc = mngr_->getComponent<HealthComponent>(e);

	drawImageWithFrames(iwf);
	drawHealth(hc);
	//draw(tr, tex);
}

void RenderSystem::drawGhosts()
{
	for (auto e : mngr_->getEntities(ecs::grp::GHOSTS)) 
	{
		auto iwf = mngr_->getComponent<ImageWithFrames>(e);
		drawImageWithFrames(iwf);
	}
}

void RenderSystem::drawMsgs() {
	// draw the score
	//
	//auto score = mngr_->getSystem<GameCtrlSystem>()->getScore();

	//Texture scoreTex(sdlutils().renderer(), std::to_string(score),
	//		sdlutils().fonts().at("ARIAL24"), build_sdlcolor(0x444444ff));

	//SDL_Rect dest = build_sdlrect( //
	//		(sdlutils().width() - scoreTex.width()) / 2.0f, //
	//		10.0f, //
	//		scoreTex.width(), //
	//		scoreTex.height());

	//scoreTex.render(dest);

	//// draw add stars message
	//sdlutils().msgs().at("addstars").render(10, 10);

}

void RenderSystem::drawImageWithFrames(ImageWithFrames* image)
{
	if (image->firstFrame != image->lastFrame) // si la imagen no es estática
		if (image->frameTimer + FRAME_DURATION 
			< sdlutils().virtualTimer().currTime()) 
		{
			image->frameTimer = sdlutils().virtualTimer().currTime();
			image->currentFrame++;
			if (image->currentFrame > image->lastFrame) {
				image->currentFrame = image->firstFrame;
			}
		}

	SDL_Rect src = build_sdlrect(
	image->currentFrame % image->nCols_ * image->frameWidth_, 
	image->currentFrame / image->nCols_ * image->frameHeight_, 
	image->frameWidth_, image->frameHeight_
	);
	SDL_Rect dst = build_sdlrect(
		image->transform_->pos_, 
		image->transform_->width_, 
		image->transform_->height_);
	image->image_->render(src, dst, image->transform_->rot_);
}

void RenderSystem::drawHealth(HealthComponent* hc)
{
	hc->render();
}

void RenderSystem::draw(Transform *tr, Texture *tex) {
	SDL_Rect dest = build_sdlrect(tr->pos_, tr->width_, tr->height_);

	assert(tex != nullptr);
	tex->render(dest, tr->rot_);
}
