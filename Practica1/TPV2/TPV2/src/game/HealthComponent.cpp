#include "HealthComponent.h"
#include <SDL_rect.h>
#include "../sdlutils/Texture.h"

constexpr int SIZE = 40,
			PADDING = 10;

HealthComponent::HealthComponent(Texture* img, int lives)
	: image_(img), lives_(lives), iniLives_(lives)
{

}

void HealthComponent::initComponent()
{
	Component::initComponent();
}

void HealthComponent::render()
{
	for (int i = 0; i < lives_; i++)
	{
		auto pos = Vector2D(PADDING + i*(SIZE+PADDING),PADDING);
		SDL_Rect dest = build_sdlrect(pos, 
			SIZE, SIZE);
		image_->render(dest);
	}
}
