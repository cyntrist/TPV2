#include "ImageWithFrames.h"
#include "../ecs/Manager.h"
ImageWithFrames::ImageWithFrames(Texture* img) :
	image_(img)
{
}

ImageWithFrames::~ImageWithFrames()
{
	delete image_;
}

void ImageWithFrames::initComponent()
{
	transform_ = mngr_->getComponent<Transform>(ent_);
	assert(transform_ != nullptr);
}

void ImageWithFrames::render()
{
	const SDL_Rect srce = build_sdlrect(
		transform_->getPos(), 
		transform_->getWidth(), 
		transform_->getHeight()
	);
	const SDL_Rect dest = build_sdlrect(
		transform_->getPos(), 
		transform_->getWidth(), 
		transform_->getHeight()
	);
	image_->render(srce, dest, transform_->getRot());
}

