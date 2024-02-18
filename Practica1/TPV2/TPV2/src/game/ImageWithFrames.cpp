#include "ImageWithFrames.h"
#include "../ecs/Manager.h"
#include "../sdlutils/SDLUtils.h"
constexpr int FRAME_SPEED = 100;

ImageWithFrames::ImageWithFrames(Texture* img, int rows, int cols) :
	image_(img),
	transform_(nullptr),
	currentRow_(0),
	currentCol_(0),
	nRows_(rows),
	nCols_(cols)
{
	frameWidth_ = image_->width() / nRows_;
	frameHeight_ = image_->height() / nCols_;
	frameTimer = sdlutils().currRealTime();
}

ImageWithFrames::~ImageWithFrames()
{
}

void ImageWithFrames::initComponent()
{
	transform_ = mngr_->getComponent<Transform>(ent_);
	assert(transform_ != nullptr);
}

void ImageWithFrames::render()
{
	if(sdlutils().currRealTime() > frameTimer + FRAME_SPEED)
	{
		currentCol_ = (currentCol_ + 1) % (nCols_ - 1);
		currentRow_ = (currentRow_ + 1) % (nRows_ - 1);
		frameTimer = sdlutils().currRealTime();
	}

	auto frame = Vector2D(currentCol_ * frameWidth_, currentRow_ * frameHeight_);
	const SDL_Rect srce = build_sdlrect(
		frame, 
		frameWidth_, 
		frameHeight_
	);
	const SDL_Rect dest = build_sdlrect(
		transform_->getPos(), 
		transform_->getWidth(), 
		transform_->getHeight()
	);
	image_->render(srce, dest, transform_->getRot());
}

