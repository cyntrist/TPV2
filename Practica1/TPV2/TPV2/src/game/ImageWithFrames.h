#pragma once
#include "Transform.h"
#include "../ecs/Component.h"
#include "../sdlutils/Texture.h"

class ImageWithFrames : public ecs::Component
{
	Texture* image_;
	Transform* transform_;

	int currentRow_; // fila actual
	int currentCol_; // columna actual

	int frameWidth_; // anchura de los frames
	int frameHeight; // altura de los frames

	int nRows_; // número de filas
	int nCols_; // número de columnas

	int frameTimer; // contador del tiempo

public:
	__CMPID_DECL__(ecs::cmp::IMAGEWITHFRAMES)

	ImageWithFrames(Texture*);
	~ImageWithFrames() override;
	void initComponent() override;
	void render() override;
};
